#include "PromotionSquare.h"

PromotionSquare::PromotionSquare(Color color) : ChessPiece(color) {
    pieceSprite = loadPiece();
}

PromotionSquare::~PromotionSquare() { }


std::string PromotionSquare::getPieceType() {
    return "PROMOTION";
}