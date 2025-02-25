#pragma once

#include "ChessPiece.h"

class Queen : public ChessPiece {
    public:
        bool isValidMove(Board *board, int new_x, int new_y);
        std::string getPieceType();
        Queen(Color color);
        ~Queen();

};