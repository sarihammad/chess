#include "ChessGame.h"
#include "Board.h"
#include "ChessPiece.h"
#include <iostream>

ChessGame::ChessGame() : board(new Board(WHITE)), currTurn(WHITE), movingPiece(nullptr), 
gameWindow(sf::VideoMode(800, 800), "Chess", sf::Style::Titlebar | sf::Style::Close) {
}

ChessGame::~ChessGame() { }

void ChessGame::drawBoard() {
    gameWindow.draw(board->boardSprite);
    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            if (board->getPieceAt(x, y) == nullptr) continue;
            gameWindow.draw(board->getPieceAt(x, y)->pieceSprite);
        } 
    }
}