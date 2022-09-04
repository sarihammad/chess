#include "ChessPiece.h"

class Board;

class King : public ChessPiece {
    public:
        bool moved;
        bool isValidMove(Board *board, int new_x, int new_y);
        void movePiece(Board *board, int new_x, int new_y);
        bool canCastleQueenside(Board *board);
        bool canCastleKingside(Board *board);
        std::string getPieceType();
        King(Color pieceColor);
        ~King();
};