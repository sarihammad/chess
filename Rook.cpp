#include "Rook.h"

Rook::Rook(Color color, bool isQueenside) : ChessPiece(color), moved(false), isQueenside(isQueenside) {
    pieceSprite = loadPiece();
}

Rook::~Rook() { }

bool Rook::isValidMove(Board *board, int new_x, int new_y) {
    int x = getPiecePosition(board).first;
    int y = getPiecePosition(board).second;
    if (x < 0 || x > 7 || y < 0 || y > 7 || new_x < 0 || new_x > 7 || new_y < 0 || new_y > 7) return false;
    if (x == new_x && y == new_y) return false;
    if (x != new_x && y != new_y) return false;
    if (pieceColor == board->getColorAt(new_x, new_y)) return false;
    if (x == new_x) {
        for (int i = std::min(y, new_y) + 1; i < std::max(y, new_y); i++) {
            if (board->getPieceAt(x, i) != nullptr) return false;
        }
        return true;
    } else {
        for (int j = std::min(x, new_x) + 1; j < std::max(x, new_x); j++) {
            if (board->getPieceAt(j, y) != nullptr) return false;
        }
        return true;
    }
}

void Rook::movePiece(Board *board, int new_x, int new_y) {
    ChessPiece::movePiece(board, new_x, new_y);
    moved = true;
}

std::string Rook::getPieceType() {
    return "ROOK";
}

// sf::Sprite Rook::loadPiece() {
//     // pieceTexture = new sf::Texture;
//     // std::string imgFile = colorToString(pieceColor) + "_" + getPieceType() + ".png";
//     // pieceTexture->loadFromFile(imgFile);
//     // pieceSprite.setTexture(*pieceTexture);
//     // return pieceSprite;
// }