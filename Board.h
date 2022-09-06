#pragma once

#include "Board.h"
#include <SFML/Graphics.hpp>
#include "helpers.cpp"
#include "Color.h"

class ChessPiece;

class Board {
    private:
        // ChessPiece *initialBoardState[8][8];
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
        ChessPiece *getPieceAt(int x, int y);
        std::string getTypeAt(int x, int y);
        Color getColorAt(int x, int y);
        sf::Sprite loadBoard();
        
};