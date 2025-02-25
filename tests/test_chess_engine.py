import pytest
import chess
import torch
from models.cnn import ChessCNN
from scripts.chess_dataset import ChessDataset
import json

def test_cnn_model():
    model = ChessCNN()
    batch_size = 4
    input_tensor = torch.randn(batch_size, 12, 8, 8)
    output = model(input_tensor)
    assert output.shape == (batch_size, 1)
    assert torch.all(output >= -1) and torch.all(output <= 1)

def test_basic_chess_rules():
    board = chess.Board()
    
    # Test initial position
    assert len(list(board.legal_moves)) == 20
    
    # Test checkmate position
    board.set_fen("rnb1kbnr/pppp1ppp/8/4p3/6Pq/5P2/PPPPP2P/RNBQKBNR w KQkq - 1 3")
    assert board.is_checkmate()
    
    # Test stalemate position
    board.set_fen("k7/8/1Q6/8/8/8/8/K7 b - - 0 1")
    assert board.is_stalemate()

def test_dataset_loading():
    dataset = ChessDataset('tests/test_data')
    assert len(dataset) > 0
    position, evaluation = dataset[0]
    assert position.shape == (12, 8, 8)
    assert -1 <= evaluation <= 1

def test_opening_book():
    with open('../resources/opening_book.json', 'r') as f:
        book = json.load(f)
    assert len(book) > 0
    
    # Test common opening position
    initial_fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
    assert initial_fen in book
    
    # Test common first moves are present
    e4_moves = book[initial_fen].get("e2e4")
    assert e4_moves is not None
    assert e4_moves["frequency"] > 0

if __name__ == '__main__':
    pytest.main([__file__]) 