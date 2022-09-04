#include "ChessPiece.h"
class Board;


class Pawn : public ChessPiece {
    public:
        bool isEnPassantPiece;
        bool isValidMove(Board *board, int new_x, int new_y);
        std::string getPieceType();
        Pawn(Color pieceColor);
        ~Pawn();

};