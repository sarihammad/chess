#include "King.h"
#include "Rook.h"

King::King(Color color) : ChessPiece(color), moved(false) {
    pieceSprite = loadPiece();
}

King::~King() { }

bool King::isValidMove(Board *board, int new_x, int new_y) {
    int x = getPiecePosition(board).first;
    int y = getPiecePosition(board).second;
    if (x < 0 || x > 7 || y < 0 || y > 7 || new_x < 0 || new_x > 7 || new_y < 0 || new_y > 7) return false;
    if (x == new_x && y == new_y) return false;
    if (pieceColor == board->getColorAt(new_x, new_y)) return false;

    if (board->bottomPlayer == WHITE) {
        if (new_x == x - 2 && new_y == y && canCastleQueenside(board)) return true;
        if (new_x == x + 2 && new_y == y && canCastleKingside(board)) return true;
    } else {
        if (new_x == x - 2 && new_y == y && canCastleKingside(board)) return true;
        if (new_x == x + 2 && new_y == y && canCastleQueenside(board)) return true;
    }

    return std::max(abs(x - new_x), abs(y - new_y)) == 1;


}

void King::movePiece(Board *board, int new_x, int new_y) {
    int x = getPiecePosition(board).first;
    Rook *castlingRook;
    if (board->bottomPlayer == WHITE) {
        if (new_x - x == 2 && canCastleKingside(board)) {
            castlingRook = (Rook*) board->getPieceAt(7, new_y);
            castlingRook->movePiece(board, new_x - 1, new_y);
        } else if (x - new_x == 2 && canCastleQueenside(board)) {
            castlingRook = (Rook*) board->getPieceAt(0, new_y);
            castlingRook->movePiece(board, new_x + 1, new_y);
        }
    } else {
        if (x - new_x == 2 && canCastleKingside(board)) {
            castlingRook = (Rook*) board->getPieceAt(0, new_y);
            castlingRook->movePiece(board, new_x + 1, new_y);
        } else if (new_x - x == 2 && canCastleQueenside(board)) {
            castlingRook = (Rook*) board->getPieceAt(7, new_y);
            castlingRook->movePiece(board, new_x - 1, new_y);
        }
    }
    ChessPiece::movePiece(board, new_x, new_y);
    moved = true;
}

bool King::canCastleQueenside(Board *board) {
    if (moved) return false;
    int kingX = getPiecePosition(board).first;
    int kingY = getPiecePosition(board).second;
    Rook *queensideRook;

    if (board->bottomPlayer == BLACK && board->getTypeAt(kingX+4, kingY) == "ROOK") {
        queensideRook = (Rook *) board->getPieceAt(kingX+4, kingY);
        if (queensideRook->isQueenside && !queensideRook->moved) {
            if (board->getPieceAt(kingX+1, kingY) == nullptr && board->getPieceAt(kingX+2, kingY) == nullptr && board->getPieceAt(kingX+3, kingY) == nullptr) {
                if (!board->isChecked(pieceColor) && !board->nextMoveIsChecked(this, kingX+1, kingY) && !board->nextMoveIsChecked(this, kingX+2, kingY)) {
                    return true;
                }
            }
        }
    } 
    if (board->bottomPlayer == WHITE && board->getTypeAt(kingX-4, kingY) == "ROOK") {
        queensideRook = (Rook *) board->getPieceAt(kingX-4, kingY);
        if (queensideRook->isQueenside && !queensideRook->moved) {
            if (board->getPieceAt(kingX-1, kingY) == nullptr && board->getPieceAt(kingX-2, kingY) == nullptr && board->getPieceAt(kingX-3, kingY) == nullptr) {
                if (!board->isChecked(pieceColor) && !board->nextMoveIsChecked(this, kingX-1, kingY) && !board->nextMoveIsChecked(this, kingX-2, kingY)) {
                    return true;
                }
            }
        }
    }

    return false;
}

bool King::canCastleKingside(Board *board) {
    if (moved) return false;
    int kingX = getPiecePosition(board).first;
    int kingY = getPiecePosition(board).second;
    Rook *kingsideRook;

    if (board->bottomPlayer == BLACK && board->getTypeAt(kingX-3, kingY) == "ROOK") {
        kingsideRook = (Rook *) board->getPieceAt(kingX-3, kingY);
        if (!kingsideRook->isQueenside && !kingsideRook->moved) {
            if (board->getPieceAt(kingX-1, kingY) == nullptr && board->getPieceAt(kingX-2, kingY) == nullptr) {
                if (!board->isChecked(pieceColor) && !board->nextMoveIsChecked(this, kingX-1, kingY) && !board->nextMoveIsChecked(this, kingX-2, kingY)) {
                    return true;
                }
            }
        }
    } 
    if (board->bottomPlayer == WHITE && board->getTypeAt(kingX+3, kingY) == "ROOK") {
        kingsideRook = (Rook *) board->getPieceAt(kingX+3, kingY);
        if (!kingsideRook->isQueenside && !kingsideRook->moved) {
            if (board->getPieceAt(kingX+1, kingY) == nullptr && board->getPieceAt(kingX+2, kingY) == nullptr) {
                if (!board->isChecked(pieceColor) && !board->nextMoveIsChecked(this, kingX+1, kingY) && !board->nextMoveIsChecked(this, kingX+2, kingY)) {
                    return true;
                }
            }
        }
    }

    return false;
}

std::string King::getPieceType() {
    return "KING";
}