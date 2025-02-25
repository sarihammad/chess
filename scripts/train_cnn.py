import torch
import torch.nn as nn
import torch.optim as optim
from torch.utils.data import DataLoader
from chess_dataset import ChessDataset
from models.cnn import ChessCNN

def train_model():
    # Initialize model
    model = ChessCNN()
    criterion = nn.MSELoss()
    optimizer = optim.Adam(model.parameters())
    
    # Load dataset
    train_dataset = ChessDataset('data/train')
    train_loader = DataLoader(train_dataset, batch_size=64, shuffle=True)
    
    # Training loop
    for epoch in range(100):
        for batch in train_loader:
            positions, evaluations = batch
            
            # Forward pass
            outputs = model(positions)
            loss = criterion(outputs, evaluations)
            
            # Backward pass
            optimizer.zero_grad()
            loss.backward()
            optimizer.step()
    
    # Save model
    torch.save(model.state_dict(), '../models/chess_cnn.pth')

if __name__ == '__main__':
    train_model() 