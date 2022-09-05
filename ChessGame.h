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
        sf::CircleShape possibleMoveSprite;
        sf::RectangleShape possibleCaptureMoveSprite;
        void drawBoard();
        void drawPieces();
        void drawPossibleMoves();
        void unsetEnPassantPieces();
        bool isPromoting();
        void promotePiece(sf::Vector2i position);
        bool noValidMove(Color color);
        bool isCheckmated(Color color);
        bool isStalemated(Color color);
        sf::CircleShape loadPossibleMove();
        sf::RectangleShape loadPossibleCaptureMove();
        ChessGame();
        ~ChessGame();
};