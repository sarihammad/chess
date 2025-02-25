import torch
from torch.utils.data import Dataset
import chess
import numpy as np
import os

class ChessDataset(Dataset):
    def __init__(self, data_dir):
        self.positions = []
        self.evaluations = []
        
        # Load all .npz files in the directory
        for file in os.listdir(data_dir):
            if file.endswith('.npz'):
                data = np.load(os.path.join(data_dir, file))
                self.positions.extend(data['positions'])
                self.evaluations.extend(data['evaluations'])
        
        self.positions = torch.FloatTensor(self.positions)
        self.evaluations = torch.FloatTensor(self.evaluations)
    
    def __len__(self):
        return len(self.positions)
    
    def __getitem__(self, idx):
        return self.positions[idx], self.evaluations[idx]
    
    @staticmethod
    def board_to_tensor(board):
        """Convert a chess.Board to a 12x8x8 tensor"""
        tensor = torch.zeros(12, 8, 8)
        piece_idx = {
            'P': 0, 'N': 1, 'B': 2, 'R': 3, 'Q': 4, 'K': 5,
            'p': 6, 'n': 7, 'b': 8, 'r': 9, 'q': 10, 'k': 11
        }
        
        for i in range(64):
            piece = board.piece_at(i)
            if piece:
                rank = i // 8
                file = i % 8
                tensor[piece_idx[piece.symbol()]][rank][file] = 1
        
        return tensor 