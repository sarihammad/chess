#pragma once

#include <SFML/Graphics.hpp>
#include "Utils/helpers.cpp"
#include "Color.h"
#include <stack>

class ChessPiece;

class Board {
    public:
        ChessPiece *boardState[8][8];
        Color bottomPlayer;
        Color topPlayer;
        sf::Sprite boardSprite;
        sf::Texture *boardTexture;
        Board(Color color);
        ~Board();
        void initializeBoard(Color bottomPlayer);
        bool isChecked(Color color);
        bool nextMoveIsChecked(ChessPiece *movingPiece, int new_x, int new_y);
        void flipBoard();
        ChessPiece *getPieceAt(int x, int y);
        std::string getTypeAt(int x, int y);
        Color getColorAt(int x, int y);
        sf::Sprite loadBoard();
        
};