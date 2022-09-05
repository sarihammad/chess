#include "ChessPiece.h"
class Board;


class PromotionSquare : public ChessPiece {
    public:
        // ChessPiece *getPromotionPiece();
        std::string getPieceType();
        PromotionSquare(Color pieceColor);
        ~PromotionSquare();
};