# Chessmate

A multiplayer chess game and AI engine with adaptive difficulty, built with C++, SFML, PyTorch, Flask, and AWS Lambda.

## Preview

![preview](https://user-images.githubusercontent.com/63919507/188940128-b0916b22-a747-4e29-83c7-4596eb01ab9a.gif)

## Status

This project is a work in progress. Core gameplay and AI features are being actively developed. Multiplayer and advanced AI levels are partially implemented.

## TODO

- [ ] Refactor AI evaluation pipeline
- [ ] Improve board UI and animation transitions
- [ ] Integrate CNN model into expert difficulty
- [ ] Finalize Lambda multiplayer logic

## Features

### Game Modes

- Local play against AI
- Online multiplayer via AWS Lambda
- Practice mode with move suggestions
- Tournament mode with ELO tracking

### AI Engine

- Minimax with alpha-beta pruning
- PyTorch-based Convolutional Neural Networks for position evaluation
- Adaptive difficulty scaling
- Opening book and endgame tablebase integration

### Online Features

- Turn-based multiplayer using AWS Lambda and DynamoDB
- Match history and basic analytics
- Global leaderboard

### Game Features

- SFML graphical interface with animations
- Complete chess rules and move validation
- Move suggestions and basic in-game analysis
- Game history with notation
- Save/load functionality
- Board and piece customization

## Setup

### Requirements

- C++17
- SFML 2.6.1
- Python 3.8
- PyTorch 2.0
- Flask (for AI server)
- AWS CLI (for multiplayer)

### Local Development

1. Train the CNN model using the Python scripts in `/scripts`
2. Run the Flask AI server locally:

```
cd server
python app.py
```
3. Launch the C++ game from `/src`
4. The game communicates with the AI server via HTTP to get move evaluations

## AI Levels

1. **Beginner** (Elo ~1200)
- 2-ply Minimax
- Basic evaluation

2. **Intermediate** (Elo ~1600)
- 4-ply search
- Alpha-beta pruning

3. **Expert** (Elo ~2000)
- 6-ply search
- CNN-based evaluation
- Opening book integration

## Project Structure

- `/src`: C++ source files (game engine and UI)
- `/include`: C++ header files
- `/models`: Trained PyTorch models (.pt files)
- `/resources`: Game assets (sprites, fonts, sounds)
- `/scripts`: Python training and evaluation scripts
- `/server`: Flask server to serve the AI model
- `/lambda`: AWS Lambda functions for multiplayer backend

## Notes

- This project is built for learning and demonstration purposes
- Model inference is done via a Flask API to keep integration simple
- Multiplayer is handled with serverless functions using AWS Lambda and DynamoDB
