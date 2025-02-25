#!/bin/bash

# Create deployment package
cd lambda
zip -r ../deployment.zip .

# Deploy Lambda functions
aws lambda update-function-code \
    --function-name chess-matchmaking \
    --zip-file fileb://../deployment.zip

aws lambda update-function-code \
    --function-name chess-game-move \
    --zip-file fileb://../deployment.zip

# Create DynamoDB tables if they don't exist
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

# Clean up
rm ../deployment.zip 