#pragma once

#include "ChessPiece.h"

class Bishop : public ChessPiece {
    public:
        bool isValidMove(Board *board, int new_x, int new_y);
        std::string getPieceType();
        Bishop(Color pieceColor);
        ~Bishop();

};