import chess
import chess.pgn
import json
from collections import defaultdict
from tqdm import tqdm

def process_game(game, book, min_frequency=10):
    board = game.board()
    for move in game.mainline_moves():
        fen = board.fen()
        str_move = move.uci()
        book[fen][str_move] += 1
        board.push(move)
        if board.fullmove_number > 10:  # Only store first 10 moves
            break

def generate_opening_book(pgn_file, output_file):
    book = defaultdict(lambda: defaultdict(int))
    
    # Process games
    with open(pgn_file) as f:
        while True:
            try:
                game = chess.pgn.read_game(f)
                if game is None:
                    break
                process_game(game, book)
            except Exception as e:
                print(f"Error processing game: {e}")
                continue
    
    # Filter and format book
    filtered_book = {}
    for position, moves in book.items():
        total_frequency = sum(moves.values())
        if total_frequency >= 10:  # Only keep positions that appear frequently
            filtered_book[position] = {
                move: {
                    'frequency': freq,
                    'probability': freq / total_frequency
                }
                for move, freq in moves.items()
            }
    
    # Save to file
    with open(output_file, 'w') as f:
        json.dump(filtered_book, f)

if __name__ == '__main__':
    generate_opening_book('data/games.pgn', '../resources/opening_book.json') 