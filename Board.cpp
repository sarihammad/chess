
#include "Board.h"
#include "Rook.h"
#include "Knight.h"
#include "Bishop.h"
#include "Queen.h"
#include "King.h"
#include "Pawn.h"
// #include "Color.h"
#include "constants.cpp"
#include <iostream>
// #include <SFML/Graphics.hpp>

Board::Board(Color color) : bottomPlayer(color), topPlayer(getOtherColor(color)) {
    Board::initializeBoard(color);
    boardSprite = loadBoard();
    

}

Board::~Board() { }

void Board::initializeBoard(Color bottomPlayer) {
    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            switch (y)
            {
            case 0:
                if (x == 0) {
                    if (bottomPlayer == WHITE) {
                        boardState[y][x] = new Rook(topPlayer, true);
                    } else {
                        boardState[y][x] = new Rook(topPlayer, false);
                    }
                } else if (x == 7) {
                    if (bottomPlayer == WHITE) {
                        boardState[y][x] = new Rook(topPlayer, false);
                    } else {
                        boardState[y][x] = new Rook(topPlayer, true);
                    }
                } else if (x == 1 || x == 6) {
                    boardState[y][x] = new Knight(topPlayer);
                } else if (x == 2 || x == 5) {
                    boardState[y][x] = new Bishop(topPlayer);
                } else if (x == 3) {
                    if (bottomPlayer == WHITE) {
                        boardState[y][x] = new Queen(topPlayer);
                    } else {
                        boardState[y][x] = new King(topPlayer);
                    }
                } else {
                    if (bottomPlayer == WHITE) {
                        boardState[y][x] = new King(topPlayer);
                    } else {
                        boardState[y][x] = new Queen(topPlayer);
                    }
                }
                break;
            case 1:
                boardState[y][x] = new Pawn(topPlayer);
                break;
            case 6:
                boardState[y][x] = new Pawn(bottomPlayer);
                break;
            case 7:
                if (x == 0) {
                    if (bottomPlayer == WHITE) {
                        boardState[y][x] = new Rook(bottomPlayer, true);
                    } else {
                        boardState[y][x] = new Rook(bottomPlayer, false);
                    }
                } else if (x == 7) {
                    if (bottomPlayer == WHITE) {
                        boardState[y][x] = new Rook(bottomPlayer, false);
                    } else {
                        boardState[y][x] = new Rook(bottomPlayer, true);
                    }
                } else if (x == 1 || x == 6) {
                    boardState[y][x] = new Knight(bottomPlayer);
                } else if (x == 2 || x == 5) {
                    boardState[y][x] = new Bishop(bottomPlayer);
                } else if (x == 3) {
                    if (bottomPlayer == WHITE) {
                        boardState[y][x] = new Queen(bottomPlayer);
                    } else {
                        boardState[y][x] = new King(bottomPlayer);
                    }
                } else {
                    if (bottomPlayer == WHITE) {
                        boardState[y][x] = new King(bottomPlayer);
                    } else {
                        boardState[y][x] = new Queen(bottomPlayer);
                    }
                }
                break;
            default:
                boardState[y][x] = nullptr;
            }
            if (getPieceAt(x, y) != nullptr) boardState[y][x]->pieceSprite.setPosition(x*size, y*size);
        }
    }
}

std::string Board::getTypeAt(int x, int y) {
    if (boardState[y][x] == nullptr) {
        return "EMPTY";
    }
    return boardState[y][x]->getPieceType();
}

ChessPiece *Board::getPieceAt(int x, int y) {
    return boardState[y][x];
}

Color Board::getColorAt(int x, int y) {
    if (boardState[y][x] == nullptr) {
        return NONE;
    }
    return boardState[y][x]->pieceColor;
}

bool Board::isChecked(Color color) {
    int kingX;
    int kingY;
    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            if (getTypeAt(x, y) == "KING" && getColorAt(x, y) == color) {
                kingX = x;
                kingY = y;
            }
        }
    }
    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            if (getPieceAt(x, y) == nullptr) continue;
            if (getColorAt(x, y) == getOtherColor(color) && getPieceAt(x, y)->isValidMove(this, kingX, kingY)) return true;
        }
    }
    return false;
}

bool Board::nextMoveIsChecked(ChessPiece *movingPiece, int new_x, int new_y) {
    bool nextMoveInCheck;
    // if (movingPiece->isValidMove(this, new_x, new_y)) {
    //     int x = movingPiece->getPiecePosition(this).first;
    //     int y = movingPiece->getPiecePosition(this).second;
    //     ChessPiece *newSquarePiece = getPieceAt(new_x, new_y);
    //     movingPiece->movePiece(this, new_x, new_y);
    //     nextMoveInCheck = isChecked(movingPiece->pieceColor);
    //     movingPiece->movePiece(this, x, y);
    //     newSquarePiece->movePiece(this, new_x, new_y);
    //     return nextMoveInCheck;
    // }
    // return false;
    int x = movingPiece->getPiecePosition(this).first;
    int y = movingPiece->getPiecePosition(this).second;
    ChessPiece *newSquarePiece = getPieceAt(new_x, new_y);
    // movingPiece->movePiece(this, new_x, new_y);
    // // nextMoveInCheck = isChecked(movingPiece->pieceColor);
    // movingPiece->movePiece(this, x, y);
    // newSquarePiece->movePiece(this, new_x, new_y);
    // return nextMoveInCheck;
    return false;
}

sf::Sprite Board::loadBoard() {
    // std::cout << "HI" << std::endl;
    // std::cout << "HI" << std::endl;
    boardTexture = new sf::Texture;
    // sf::Texture *text;
    boardTexture->loadFromFile("images/BOARD.png");
    boardSprite.setTexture(*boardTexture);
    return boardSprite;
}