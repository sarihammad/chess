import torch
import torch.nn as nn
import torch.optim as optim
import numpy as np
from torch.utils.data import Dataset, DataLoader

class ChessNet(nn.Module):
    def __init__(self):
        super(ChessNet, self).__init__()
        self.conv1 = nn.Conv2d(12, 64, 3, padding=1)
        self.conv2 = nn.Conv2d(64, 128, 3, padding=1)
        self.conv3 = nn.Conv2d(128, 256, 3, padding=1)
        self.fc1 = nn.Linear(256 * 8 * 8, 1024)
        self.fc2 = nn.Linear(1024, 1)
        
    def forward(self, x):
        x = torch.relu(self.conv1(x))
        x = torch.relu(self.conv2(x))
        x = torch.relu(self.conv3(x))
        x = x.view(-1, 256 * 8 * 8)
        x = torch.relu(self.fc1(x))
        x = torch.tanh(self.fc2(x))  # Output between -1 and 1
        return x

class ChessDataset(Dataset):
    def __init__(self, positions, scores):
        self.positions = positions
        self.scores = scores
        
    def __len__(self):
        return len(self.positions)
        
    def __getitem__(self, idx):
        return self.positions[idx], self.scores[idx]

def train_model(model, train_loader, num_epochs=100):
    criterion = nn.MSELoss()
    optimizer = optim.Adam(model.parameters())
    
    for epoch in range(num_epochs):
        for positions, scores in train_loader:
            optimizer.zero_grad()
            outputs = model(positions)
            loss = criterion(outputs, scores)
            loss.backward()
            optimizer.step()
            
        print(f'Epoch {epoch+1}, Loss: {loss.item():.4f}')
    
    return model

# Train three different models with different datasets/parameters
def train_models():
    beginner = ChessNet()
    intermediate = ChessNet()
    expert = ChessNet()
    
    # Train with different datasets/parameters
    train_model(beginner, beginner_loader, num_epochs=50)
    train_model(intermediate, intermediate_loader, num_epochs=100)
    train_model(expert, expert_loader, num_epochs=200)
    
    # Export models
    torch.jit.script(beginner).save("models/beginner.pt")
    torch.jit.script(intermediate).save("models/intermediate.pt")
    torch.jit.script(expert).save("models/expert.pt") 