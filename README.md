# Chessmate

A multiplayer chess game and AI engine with adaptive difficulty, built with C++, SFML, PyTorch, and AWS Lambda.

## Preview

![preview](https://user-images.githubusercontent.com/63919507/188940128-b0916b22-a747-4e29-83c7-4596eb01ab9a.gif)

## Features

### Game Modes

- Local play against AI
- Online multiplayer via AWS Lambda
- Practice mode with move suggestions
- Tournament mode with ELO tracking

### AI Engine

- Minimax with alpha-beta pruning (40% faster move calculation)
- PyTorch-based Convolutional Neural Networks for position evaluation
- Adaptive difficulty system
- Opening book and endgame tablebase integration

### Online Features

- Real-time multiplayer via AWS Lambda
- Low-latency gameplay (< 100ms)
- Auto-scaling server infrastructure
- Match history and analytics
- Global leaderboard

### Game Features

- SFML graphical interface with smooth animations
- Complete chess rules and move validation
- In-game analysis and move suggestions
- Game history with notation
- Save/load functionality
- Board customization

## Setup

### Requirements

- C++17
- SFML 2.6.1
- Python 3.8
- PyTorch 2.0
- AWS CLI (for multiplayer)

## AI Levels

1. **Beginner** (Elo ~1200)

   - 2-ply search
   - Basic evaluation

2. **Intermediate** (Elo ~1600)

   - 4-ply search
   - Alpha-beta pruning

3. **Expert** (Elo ~2000)

   - 6-ply search
   - CNN evaluation
   - Opening book


## Project Structure

- `/src`: C++ source files
- `/include`: Header files
- `/models`: Neural network models
- `/resources`: Game assets
- `/scripts`: Python training scripts
- `/lambda`: AWS Lambda functions
- `/server`: Multiplayer server code

## Performance

- Move calculation: 40% faster with alpha-beta pruning
- Average server response: < 100ms
- CNN evaluation: 200ms per position
- Auto-scaling up to 10,000 concurrent games


