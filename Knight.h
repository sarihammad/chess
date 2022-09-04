#include "ChessPiece.h"
class Board;


class Knight : public ChessPiece {
    public:
        bool isValidMove(Board *board, int new_x, int new_y);
        std::string getPieceType();
        Knight(Color pieceColor);
        ~Knight();
};