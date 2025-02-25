import asyncio
import websockets
import json
import boto3
from datetime import datetime

# AWS Lambda client for game logic
lambda_client = boto3.client('lambda')

# Active connections
connections = {}
games = {}

async def handle_connection(websocket, path):
    try:
        # Register connection
        connection_id = str(datetime.now().timestamp())
        connections[connection_id] = websocket
        
        async for message in websocket:
            data = json.loads(message)
            
            if data['action'] == 'find_match':
                # Forward to Lambda for matchmaking
                response = lambda_client.invoke(
                    FunctionName='chess-matchmaking',
                    InvocationType='RequestResponse',
                    Payload=json.dumps(data)
                )
                await websocket.send(response['Payload'].read())
                
            elif data['action'] == 'make_move':
                # Validate and broadcast move
                game_id = data['game_id']
                if game_id in games:
                    # Forward move to Lambda
                    lambda_client.invoke(
                        FunctionName='chess-game-move',
                        InvocationType='Event',
                        Payload=json.dumps(data)
                    )
                    
                    # Broadcast to opponent
                    opponent_socket = games[game_id]['players']['white' if data['color'] == 'black' else 'black']
                    await opponent_socket.send(json.dumps({
                        'type': 'move',
                        'move': data['move']
                    }))
    
    finally:
        # Cleanup on disconnect
        if connection_id in connections:
            del connections[connection_id]

async def main():
    async with websockets.serve(handle_connection, "localhost", 8765):
        await asyncio.Future()  # run forever

if __name__ == "__main__":
    asyncio.run(main()) 