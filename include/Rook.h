#pragma once

#include "ChessPiece.h"

class Rook : public ChessPiece {
    public:
        bool moved;
        bool isQueenside;
        bool isValidMove(Board *board, int new_x, int new_y);
        void movePiece(Board *board, int new_x, int new_y);
        std::string getPieceType();
        Rook(Color pieceColor, bool isQueenside);
        ~Rook();
};