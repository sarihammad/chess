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

    if (!moved) {
        if (pieceColor == board->bottomPlayer) {
            if (x == 4) {
                if (new_x == x - 2 && new_y == y && canCastleQueenside(board)) return true;
                if (new_x == x + 2 && new_y == y && canCastleKingside(board)) return true;
            }
            if (x == 3) {
                if (new_x == x + 2 && new_y == y && canCastleQueenside(board)) return true;
                if (new_x == x - 2 && new_y == y && canCastleKingside(board)) return true;
            }
 
        }
    }
    // if (abs(x - new_x) != 1 || abs(y - new_y) != 1) {
    if (pieceColor == board->getColorAt(new_x, new_y)) return false;
    return std::max(abs(x - new_x), abs(y - new_y)) == 1;
    // }


    // return true;
}

void King::movePiece(Board *board, int new_x, int new_y) {
    int x = getPiecePosition(board).first;
    ChessPiece::movePiece(board, new_x, new_y);
    if (isValidMove(board, new_x, new_y) && abs(new_x - x) == 2 && canCastleKingside(board) && (new_x == 1 || new_x == 7)) {
        Rook *kingsideRook = new_x == 1 ? (Rook*) board->getPieceAt(0, new_y) : (Rook*) board->getPieceAt(7, new_y);
        if (new_x == 1) {
            kingsideRook->movePiece(board, new_x+2, new_y);
        } else {
            kingsideRook->movePiece(board, new_x-2, new_y);
        }
    }
    if (isValidMove(board, new_x, new_y) && abs(new_x - x) == 2 && canCastleQueenside(board) && (new_x == 2 || new_x == 5)) {
        Rook *queensideRook = new_x == 2 ? (Rook*) board->getPieceAt(0, new_y) : (Rook*) board->getPieceAt(7, new_y);
        if (new_x == 2) {
            queensideRook->movePiece(board, new_x+3, new_y);
        } else {
            queensideRook->movePiece(board, new_x-3, new_y);
        }
    }
    moved = true;
}

bool King::canCastleQueenside(Board *board) {
    if (moved) return false;
    int kingX = getPiecePosition(board).first;
    int kingY = getPiecePosition(board).second;
    Rook *queensideRook;

    if (kingX == 3 && board->getTypeAt(kingX+4, kingY) == "ROOK") {
        queensideRook = (Rook *) board->getPieceAt(kingX+4, kingY);
        if (queensideRook->isQueenside && !queensideRook->moved) {
            if (board->getPieceAt(kingX+1, kingY) == nullptr && board->getPieceAt(kingX+2, kingY) == nullptr && board->getPieceAt(kingX+3, kingY) == nullptr) {
                if (!board->isChecked(pieceColor) && !board->nextMoveIsChecked(this, kingX+1, kingY) && !board->nextMoveIsChecked(this, kingX+2, kingY)) {
                    return true;
                }
            }
        }
    } 
    if (kingX == 4 && board->getTypeAt(kingX-4, kingY) == "ROOK") {
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

    // if (kingX == 3) queensideRook = 

    if (kingX == 3 && board->getTypeAt(kingX-3, kingY) == "ROOK") {
        kingsideRook = (Rook *) board->getPieceAt(kingX-3, kingY);
        if (!kingsideRook->isQueenside && !kingsideRook->moved) {
            if (board->getPieceAt(kingX-1, kingY) == nullptr && board->getPieceAt(kingX-2, kingY) == nullptr) {
                if (!board->isChecked(pieceColor) && !board->nextMoveIsChecked(this, kingX-1, kingY) && !board->nextMoveIsChecked(this, kingX-2, kingY)) {
                    return true;
                }
            }
        }
    } 
    if (kingX == 4 && board->getTypeAt(kingX+3, kingY) == "ROOK") {
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

// sf::Sprite King::loadPiece() {
//     pieceTexture = new sf::Texture;
//     // std::string imgFile = piece->pieceColor + "_" + piece->pieceType + ".png";
//     pieceTexture->loadFromFile("images/WHITE_KING.png");
//     pieceSprite.setTexture(*pieceTexture);
//     return pieceSprite;
// }
