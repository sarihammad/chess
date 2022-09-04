// #include "Board.h"
#include <SFML/Graphics.hpp>
#include "Color.h"
#include "Board.h"


class Board;
class ChessPiece;

class ChessGame {
    public:
        Board *board;
        Color currTurn;
        ChessPiece *movingPiece;
        sf::RenderWindow gameWindow;
        void drawBoard();
        ChessGame();
        ~ChessGame();
};