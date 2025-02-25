#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

struct Move {
    int fromX, fromY;
    int toX, toY;
    bool isCastling;
    bool isEnPassant;
    bool isPromotion;
    char promotionPiece;
    
    Move(int fx, int fy, int tx, int ty) 
        : fromX(fx), fromY(fy), toX(tx), toY(ty),
          isCastling(false), isEnPassant(false), 
          isPromotion(false), promotionPiece(' ') {}
};

class Piece {
public:
    Piece(Color color, const std::string& texturePath);
    virtual ~Piece() = default;
    
    virtual bool isValidMove(const Board& board, const Move& move) const = 0;
    virtual std::vector<Move> getPossibleMoves(const Board& board) const = 0;
    
    void setPosition(int x, int y);
    void draw(sf::RenderWindow& window);
    Color getColor() const { return color; }
    
protected:
    Color color;
    sf::Texture texture;
    sf::Sprite sprite;
    int posX, posY;
    
    bool isPathClear(const Board& board, const Move& move) const;
}; 