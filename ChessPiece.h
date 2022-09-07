#pragma once

#include "Utils/constants.cpp"
#include <SFML/Graphics.hpp>
#include "Color.h"
#include "Board.h"

// Abstract class for a chess piece
class ChessPiece {
    public:
        Color pieceColor;
        // std::string pieceType;
        // static std::pair<int, int> piecePosition;
        sf::Sprite pieceSprite;
        sf::Texture *pieceTexture;
        // virtual sf::Sprite setUpSprite();
        virtual bool isValidMove(Board *board, int new_x, int new_y);
        virtual std::string getPieceType();
        virtual void movePiece(Board *board, int new_x, int new_y);
        std::pair<int, int> getPiecePosition(Board *board);
        sf::Sprite loadPiece();
        ChessPiece(Color color);
        ~ChessPiece();
};