#include "ChessPiece.h"
// #include <ImageLoader.cpp"
#include "constants.cpp"
#include <iostream>



ChessPiece::ChessPiece(Color color) : pieceColor(color) {
}

ChessPiece::~ChessPiece() { }

void ChessPiece::movePiece(Board *board, int new_x, int new_y) {
    int x = getPiecePosition(board).first;
    int y = getPiecePosition(board).second;
    board->boardState[y][x] = nullptr;
    board->boardState[new_y][new_x] = this;
    pieceSprite.setPosition(new_x * size, new_y * size);
}

std::pair<int, int> ChessPiece::getPiecePosition(Board *board) {
    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            if (board->getPieceAt(x, y) == this) return std::pair<int, int>(x, y);
        }
    }
    return std::pair<int, int>(-1, -1);
}

bool ChessPiece::isValidMove(Board *board, int new_x, int new_y) {
    return false;
}

std::string ChessPiece::getPieceType() {
    return "EMPTY";
}

sf::Sprite ChessPiece::loadPiece() {
    if (getPieceType() != "EMPTY") {
        pieceTexture = new sf::Texture;
        std::string imgFile = "images/" + colorToString(pieceColor) + "_" + getPieceType() + ".png";
        pieceTexture->loadFromFile(imgFile);
        pieceSprite.setTexture(*pieceTexture);
    }

    return pieceSprite;
}







