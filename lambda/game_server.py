import json
import boto3
from datetime import datetime

dynamodb = boto3.resource('dynamodb')
games_table = dynamodb.Table('chess_games')
matchmaking_table = dynamodb.Table('chess_matchmaking')

def lambda_handler(event, context):
    route = event['route']
    
    if route == 'create_game':
        return create_game(event)
    elif route == 'make_move':
        return make_move(event)
    elif route == 'find_match':
        return find_match(event)
    elif route == 'update_elo':
        return update_elo(event)
    
    return {
        'statusCode': 400,
        'body': 'Invalid route'
    }

def create_game(event):
    game_id = str(datetime.now().timestamp())
    games_table.put_item(
        Item={
            'game_id': game_id,
            'white_player': event['player1'],
            'black_player': event['player2'],
            'moves': [],
            'status': 'active'
        }
    )
    return {
        'statusCode': 200,
        'body': json.dumps({'game_id': game_id})
    }

def make_move(event):
    game_id = event['game_id']
    move = event['move']
    
    games_table.update_item(
        Key={'game_id': game_id},
        UpdateExpression='SET moves = list_append(moves, :move)',
        ExpressionAttributeValues={
            ':move': [move]
        }
    )
    return {
        'statusCode': 200,
        'body': 'Move recorded'
    }

def find_match(event):
    player = event['player']
    elo = event['elo']
    
    # Find a suitable opponent
    response = matchmaking_table.scan(
        FilterExpression='abs(elo - :player_elo) < :threshold',
        ExpressionAttributeValues={
            ':player_elo': elo,
            ':threshold': 200
        }
    )
    
    if response['Items']:
        opponent = response['Items'][0]
        return create_game({
            'player1': player,
            'player2': opponent['player']
        })
    else:
        # Add to matchmaking queue
        matchmaking_table.put_item(
            Item={
                'player': player,
                'elo': elo,
                'timestamp': str(datetime.now())
            }
        )
        return {
            'statusCode': 202,
            'body': 'Added to matchmaking queue'
        }

def update_elo(event):
    # Implementation of update_elo function
    pass

    return {
        'statusCode': 200,
        'body': 'Elo update logic not implemented'
    } 