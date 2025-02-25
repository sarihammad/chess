#pragma once

#include "ChessPiece.h"

class King : public ChessPiece {
    public:
        bool moved;
        bool isValidMove(Board *board, int new_x, int new_y);
        void movePiece(Board *board, int new_x, int new_y);
        bool canCastleQueenside(Board *board);
        bool canCastleKingside(Board *board);
        bool isValidKingsideCastleMove(Board *board, int new_x, int new_y);
        void castleKingside(Board *board, int new_x, int new_y);
        bool isValidQueensideCastleMove(Board *board, int new_x, int new_y);
        void castleQueenside(Board *board, int new_x, int new_y);
        std::string getPieceType();
        King(Color pieceColor);
        ~King();
};