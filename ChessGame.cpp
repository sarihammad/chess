#include "ChessGame.h"
#include "Board.h"
#include "ChessPiece.h"
#include "Pawn.h"
#include "Rook.h"
#include "Queen.h"
#include "Bishop.h"
#include "Knight.h"
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
    possibleMoveSprite.setFillColor(sf::Color(0, 0, 0, 50));
    possibleMoveSprite.setOrigin(size/5, size/5);
    return possibleMoveSprite;
}


sf::RectangleShape ChessGame::loadPossibleCaptureMove() {
    possibleCaptureMoveSprite.setOrigin(size/2, size/2);
    possibleCaptureMoveSprite.setSize(sf::Vector2f(size, size));
    possibleCaptureMoveSprite.setFillColor(sf::Color(255, 0, 0, 255));
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

bool ChessGame::isPromoting() {
    for (int x = 0; x < 8; x++) {
        if (board->getTypeAt(x, 0) == "PROMOTION") return true;
        if (board->getTypeAt(x, 7) == "PROMOTION") return true;
    }
    return false;
}

void ChessGame::promotePiece(sf::Vector2i position) {
    int promotionX;
    int promotionY;
    for (int x = 0; x < 8; x++) {
        if (board->getTypeAt(x, 0) == "PROMOTION") {
            promotionX = x;
            promotionY = 0;
        }
        if (board->getTypeAt(x, 7) == "PROMOTION") {
            promotionX = x;
            promotionY = 7;
        }
    }
    if (position.x / size == promotionX && position.y / size == promotionY) {
        if (position.x % size < size / 2 && position.y % size < size / 2) {
            board->boardState[promotionY][promotionX] = promotionY == 0 ? new Rook(board->bottomPlayer, false) : new Rook(board->topPlayer, false);
            board->boardState[promotionY][promotionX]->pieceSprite.setPosition(promotionX * size, promotionY * size);
        } else if (position.x % size > size / 2 && position.y % size < size / 2) {
            board->boardState[promotionY][promotionX] = promotionY == 0 ? new Queen(board->bottomPlayer) : new Queen(board->topPlayer);
            board->boardState[promotionY][promotionX]->pieceSprite.setPosition(promotionX * size, promotionY * size);
        } else if (position.x % size < size / 2 && position.y % size > size / 2) {
            board->boardState[promotionY][promotionX] = promotionY == 0 ? new Bishop(board->bottomPlayer) : new Bishop(board->topPlayer);
            board->boardState[promotionY][promotionX]->pieceSprite.setPosition(promotionX * size, promotionY * size);
        } else if (position.x % size > size / 2 && position.y % size > size / 2) {
            board->boardState[promotionY][promotionX] = promotionY == 0 ? new Knight(board->bottomPlayer) : new Knight(board->topPlayer);
            board->boardState[promotionY][promotionX]->pieceSprite.setPosition(promotionX * size, promotionY * size);
        }
    }
}


bool ChessGame::noValidMove(Color color) {
    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            ChessPiece *chessPiece;
            if (board->getColorAt(x, y) != color) continue;
            chessPiece = board->getPieceAt(x, y);
            for (int i = 0; i < 8; i++) {
                for (int j = 0; j < 8; j++) {
                    if (chessPiece->isValidMove(board, j, i) && !board->nextMoveIsChecked(chessPiece, j, i)) return false;
                }
            }
        }
    }
    return true;
}

bool ChessGame::isCheckmated(Color color) {
    return noValidMove(color) && board->isChecked(color);
}

bool ChessGame::isStalemated(Color color) {
    return noValidMove(color) && !board->isChecked(color) && currTurn == color;
}