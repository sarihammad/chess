#include "ChessGame.h"
#include "Board.h"
#include "ChessPiece.h"
#include "Pawn.h"
#include "constants.cpp"
#include <iostream>

ChessGame::ChessGame() : board(new Board(WHITE)), currTurn(WHITE), movingPiece(nullptr), 
gameWindow(sf::VideoMode(800, 800), "Chess", sf::Style::Titlebar | sf::Style::Close) {
    possibleMoveSprite = loadPossibleMove();
    possibleCaptureMoveSprite = loadPossibleCaptureMove();

}

ChessGame::~ChessGame() { }

void ChessGame::drawBoard() {
    gameWindow.draw(board->boardSprite);
}

void ChessGame::drawPieces() {
    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            if (board->getPieceAt(x, y) == nullptr || board->getPieceAt(x, y) == movingPiece) continue;
            gameWindow.draw(board->getPieceAt(x, y)->pieceSprite);
        } 
    }
}

void ChessGame::unsetEnPassantPieces() {
    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            if (board->getTypeAt(x, y) == "PAWN" && currTurn == board->getColorAt(x, y))  {
                Pawn *pawnPiece = (Pawn *) board->getPieceAt(x, y);
                if (pawnPiece->isEnPassantPiece) pawnPiece->isEnPassantPiece = false;
            }
        }
    }
}

sf::CircleShape ChessGame::loadPossibleMove() {
    
    possibleMoveSprite.setRadius(size/5);
    sf::Color possibleMoveColor(0, 0, 0, 50);
    possibleMoveSprite.setFillColor(possibleMoveColor);
    possibleMoveSprite.setOrigin(size/5, size/5);
    return possibleMoveSprite;
}


sf::RectangleShape ChessGame::loadPossibleCaptureMove() {
    possibleCaptureMoveSprite.setOrigin(size/2, size/2);
    sf::Vector2f possibleCaptureMoveSize(size, size);
    sf::Color possibleCaptureMoveColor(255, 0, 0, 255);
    possibleCaptureMoveSprite.setSize(possibleCaptureMoveSize);
    possibleCaptureMoveSprite.setFillColor(possibleCaptureMoveColor);
    return possibleCaptureMoveSprite;
}

void ChessGame::drawPossibleMoves() {
    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            if (movingPiece != nullptr && movingPiece->isValidMove(board, x, y) && !board->nextMoveIsChecked(movingPiece, x, y)) {
                if (board->getColorAt(x, y) == getOtherColor(movingPiece->pieceColor)) {
                    possibleCaptureMoveSprite.setPosition(x*size + size/2, y*size + size/2);
                    gameWindow.draw(possibleCaptureMoveSprite);
                } else {
                    possibleMoveSprite.setPosition(x*size + size/2, y*size + size/2);
                    gameWindow.draw(possibleMoveSprite);
                }

            }
        } 
    }
}

