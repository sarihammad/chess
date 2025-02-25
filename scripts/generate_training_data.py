import chess
import chess.engine
import numpy as np
from chess_dataset import ChessDataset
import os
from tqdm import tqdm

def evaluate_position(engine, board, time_limit=0.1):
    """Evaluate a position using Stockfish"""
    result = engine.analyse(board, chess.engine.Limit(time=time_limit))
    score = result['score'].relative.score()
    # Normalize score to [-1, 1] range using tanh
    return np.tanh(score / 1000)

def process_game(engine, game, positions, evaluations):
    board = game.board()
    for move in game.mainline_moves():
        # Convert position to tensor
        tensor = ChessDataset.board_to_tensor(board)
        positions.append(tensor.numpy())
        
        # Get position evaluation
        eval = evaluate_position(engine, board)
        evaluations.append(eval)
        
        # Make the move
        board.push(move)

def generate_training_data(pgn_file, output_dir, max_games=1000):
    positions = []
    evaluations = []
    
    # Create output directory if it doesn't exist
    os.makedirs(output_dir, exist_ok=True)
    
    # Initialize Stockfish engine
    transport = chess.engine.SimpleEngine.popen_uci("/usr/local/bin/stockfish")
    
    try:
        # Process games
        with open(pgn_file) as f:
            for _ in tqdm(range(max_games)):
                game = chess.pgn.read_game(f)
                if game is None:
                    break
                process_game(transport, game, positions, evaluations)
    
        # Convert to numpy arrays
        positions = np.array(positions)
        evaluations = np.array(evaluations)
        
        # Save data
        output_file = os.path.join(output_dir, 'chess_positions.npz')
        np.savez(output_file, positions=positions, evaluations=evaluations)
    
    finally:
        # Always close the engine
        transport.quit()

if __name__ == '__main__':
    generate_training_data('data/games.pgn', 'data/training') 