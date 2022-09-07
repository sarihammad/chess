#include "Queen.h"

Queen::Queen(Color color) : ChessPiece(color) {
    pieceSprite = loadPiece();
}

Queen::~Queen() { }

bool Queen::isValidMove(Board *board, int new_x, int new_y) {
    int x = getPiecePosition(board).first;
    int y = getPiecePosition(board).second;
    if (x < 0 || x > 7 || y < 0 || y > 7 || new_x < 0 || new_x > 7 || new_y < 0 || new_y > 7) return false;
    if (x == new_x && y == new_y) return false;
    if (abs(x - new_x) != abs(y - new_y) && (x != new_x && y != new_y)) return false;
    if (pieceColor == board->getColorAt(new_x, new_y)) return false;

    if (abs(x - new_x) == abs(y - new_y)) {
        int currX = x < new_x ? x+1 : x-1;
        int currY = y < new_y ? y+1 : y-1;
        while (currX != new_x && currY != new_y) {
            if (board->getPieceAt(currX, currY) != nullptr) return false;
            currX < new_x ? currX++ : currX--;
            currY < new_y ? currY++ : currY--;
        }
        return true;
    }

    if (x == new_x) {
        for (int i = std::min(y, new_y) + 1; i < std::max(y, new_y); i++) {
            if (board->getPieceAt(x, i) != nullptr) return false;
        }
    } 
    if (y == new_y) {
        for (int j = std::min(x, new_x) + 1; j < std::max(x, new_x); j++) {
            if (board->getPieceAt(j, y) != nullptr) return false;
        }
    }

    return true;
}

std::string Queen::getPieceType() {
    return "QUEEN";
}

