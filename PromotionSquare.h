#pragma once

#include "ChessPiece.h"

class PromotionSquare : public ChessPiece {
    public:
        std::string getPieceType();
        PromotionSquare(Color pieceColor);
        ~PromotionSquare();
};