#!/bin/bash

# Load configuration
CONFIG_FILE="../lambda/config.json"
REGION=$(jq -r '.api.region' $CONFIG_FILE)
STAGE=$(jq -r '.api.stage' $CONFIG_FILE)

# Create API Gateway WebSocket API
API_ID=$(aws apigateway create-rest-api \
    --name "ChessGame-WebSocket" \
    --protocol-type WEBSOCKET \
    --route-selection-expression '$request.body.action' \
    --region $REGION \
    --output text \
    --query 'id')

# Create routes
aws apigateway create-resource --rest-api-id $API_ID --parent-id root --path-part 'connect'
aws apigateway create-resource --rest-api-id $API_ID --parent-id root --path-part 'disconnect'
aws apigateway create-resource --rest-api-id $API_ID --parent-id root --path-part 'default'

# Create Lambda functions
aws lambda create-function \
    --function-name chess-matchmaking \
    --runtime python3.8 \
    --handler game_server.lambda_handler \
    --role "arn:aws:iam::${AWS_ACCOUNT_ID}:role/chess-lambda-role" \
    --code S3Bucket=your-deployment-bucket,S3Key=deployment.zip

aws lambda create-function \
    --function-name chess-game-move \
    --runtime python3.8 \
    --handler game_server.lambda_handler \
    --role "arn:aws:iam::${AWS_ACCOUNT_ID}:role/chess-lambda-role" \
    --code S3Bucket=your-deployment-bucket,S3Key=deployment.zip

# Create DynamoDB tables
aws dynamodb create-table \
    --table-name chess_games \
    --attribute-definitions AttributeName=game_id,AttributeType=S \
    --key-schema AttributeName=game_id,KeyType=HASH \
    --provisioned-throughput ReadCapacityUnits=5,WriteCapacityUnits=5

aws dynamodb create-table \
    --table-name chess_matchmaking \
    --attribute-definitions AttributeName=player,AttributeType=S \
    --key-schema AttributeName=player,KeyType=HASH \
    --provisioned-throughput ReadCapacityUnits=5,WriteCapacityUnits=5

# Create IAM role for Lambda
aws iam create-role \
    --role-name chess-lambda-role \
    --assume-role-policy-document '{
        "Version": "2012-10-17",
        "Statement": [{
            "Effect": "Allow",
            "Principal": {
                "Service": "lambda.amazonaws.com"
            },
            "Action": "sts:AssumeRole"
        }]
    }'

# Attach policies to role
aws iam attach-role-policy \
    --role-name chess-lambda-role \
    --policy-arn arn:aws:iam::aws:policy/service-role/AWSLambdaBasicExecutionRole

aws iam attach-role-policy \
    --role-name chess-lambda-role \
    --policy-arn arn:aws:iam::aws:policy/AmazonDynamoDBFullAccess

# Deploy API
aws apigateway create-deployment \
    --rest-api-id $API_ID \
    --stage-name $STAGE

# Update config file with API endpoint
ENDPOINT="wss://${API_ID}.execute-api.${REGION}.amazonaws.com/${STAGE}"
jq --arg endpoint "$ENDPOINT" '.websocket.endpoint = $endpoint' $CONFIG_FILE > tmp.json && mv tmp.json $CONFIG_FILE

echo "Infrastructure setup complete!"
echo "WebSocket endpoint: $ENDPOINT" 