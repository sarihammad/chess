#include "Knight.h"

Knight::Knight(Color color) : ChessPiece(color) {
    pieceSprite = loadPiece();
}

Knight::~Knight() { }

bool Knight::isValidMove(Board *board, int new_x, int new_y) {
    int x = getPiecePosition(board).first;
    int y = getPiecePosition(board).second;
    if (x < 0 || x > 7 || y < 0 || y > 7 || new_x < 0 || new_x > 7 || new_y < 0 || new_y > 7) return false;
    if (x == new_x && y == new_y) return false;
    if (abs(new_x - x) == 2 && abs(new_y - y) == 1 || abs(new_x - x) == 1 && abs(new_y - y) == 2) {
        if (pieceColor == board->getColorAt(new_x, new_y)) {
            return false;
        } else {
            return true;
        }   
    }
    return false;
}

std::string Knight::getPieceType() {
    return "KNIGHT";
}

// sf::Sprite Knight::loadPiece() {
//     pieceTexture = new sf::Texture;
//     // std::string imgFile = piece->pieceColor + "_" + piece->pieceType + ".png";
//     pieceTexture->loadFromFile("images/WHITE_KNIGHT.png");
//     pieceSprite.setTexture(*pieceTexture);
//     return pieceSprite;
// }







