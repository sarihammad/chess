// #include "Board.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
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
        sf::SoundBuffer *moveSoundBuffer;
        sf::Sound *moveSound;
        void drawBoard();
        void drawPieces();
        void drawPossibleMoves();
        void unsetEnPassantPieces();
        bool isPromoting();
        void promotePiece(sf::Vector2i position);
        bool noValidMove(Color color);
        bool isCheckmated(Color color);
        bool isStalemated(Color color);
        bool nextMoveIsCheck(ChessPiece *movingPiece, int new_x, int new_y);
        bool nextMoveIsCheckmate(ChessPiece *movingPiece, int new_x, int new_y);
        bool nextMoveIsStalemate(ChessPiece *movingPiece, int new_x, int new_y);
        void setSoundFromMove(sf::SoundBuffer *moveSoundBuffer, sf::Sound *moveSound, ChessPiece *movingPiece, int new_x, int new_y);
        sf::CircleShape loadPossibleMove();
        sf::RectangleShape loadPossibleCaptureMove();
        ChessGame();
        ~ChessGame();
};