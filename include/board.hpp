#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "piece.hpp"

enum Color { WHITE, BLACK };

class Board {
public:
    Board();
    void initializeBoard();
    void drawBoard(sf::RenderWindow& window);
    bool makeMove(const Move& move);
    bool isValidMove(const Move& move) const;
    bool isInCheck(Color color) const;
    bool isCheckmate(Color color) const;
    bool isStalemate(Color color) const;
    std::vector<Move> getLegalMoves(Color color) const;
    Piece* getPieceAt(int x, int y) const;
    void undoLastMove();
    
private:
    std::vector<std::vector<std::unique_ptr<Piece>>> squares;
    std::vector<Move> moveHistory;
    sf::Texture boardTexture;
    sf::Sprite boardSprite;
    
    bool isSquareAttacked(int x, int y, Color byColor) const;
    void updateCastlingRights(const Move& move);
    bool isEnPassantPossible(const Move& move) const;
}; 