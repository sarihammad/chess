#include "ChessPiece.h"
class Board;


class Pawn : public ChessPiece {
    public:
        bool isEnPassantPiece;
        bool isValidMove(Board *board, int new_x, int new_y);
        bool isValidEnPassantMove(Board *board, int new_x, int new_y);
        void enPassantCapture(Board *board, int new_x, int new_y);
        void movePiece(Board *board, int new_x, int new_y);
        std::string getPieceType();
        Pawn(Color pieceColor);
        ~Pawn();

};