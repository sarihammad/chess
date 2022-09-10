#include <SFML/Graphics.hpp>
#include "ChessGame.h"
#include "ChessPiece.h"
#include <iostream>


int main() {

    ChessGame *game = new ChessGame();
    while (game->gameWindow.isOpen()) {
        sf::Vector2i position = sf::Mouse::getPosition(game->gameWindow);
        int x = position.x / size;
        int y = position.y / size;
        sf::Event e;
        while (game->gameWindow.pollEvent(e)) {
            if (e.type == sf::Event::Closed) 
                game->gameWindow.close();
            if(e.type == sf::Event::MouseButtonPressed) {
                if (e.mouseButton.button == sf::Mouse::Left) {
                    for (int i = 0; i < 8; i++) {
                        for (int j = 0; j < 8; j++) {
                            if (game->board->getPieceAt(j, i) != nullptr && x == j && y == i && game->board->getColorAt(j, i) == game->currTurn && !game->isPromoting()) {
                                game->movingPiece = game->board->getPieceAt(x, y);
                            }
                        }
                    }
                    if (game->isPromoting()) {
                        game->promotePiece(position);
                    }
                }
            }
            if (e.type == sf::Event::MouseButtonReleased) {
                if (game->movingPiece != nullptr) {
                    int movingPieceX = game->movingPiece->getPiecePosition(game->board).first;
                    int movingPieceY = game->movingPiece->getPiecePosition(game->board).second;
                    if (game->currTurn == game->movingPiece->pieceColor && game->movingPiece->isValidMove(game->board, x, y) && !game->board->nextMoveIsChecked(game->movingPiece, x, y)) {
                        game->unsetEnPassantPieces();
                        game->currTurn = getOtherColor(game->currTurn);
                        game->setSoundFromMove(game->moveSoundBuffer, game->moveSound, game->movingPiece, x, y);
                        game->moveSound->play();
                        game->movingPiece->movePiece(game->board, x, y);

                    } else {
                        game->movingPiece->pieceSprite.setPosition(size * movingPieceX, size * movingPieceY);
                    }
                    game->movingPiece = nullptr;
                }
            }

            if (e.type == sf::Event::KeyPressed) {
                if (e.key.code == sf::Keyboard::R) {
                    game->board->initializeBoard(game->board->bottomPlayer);
                    game->currTurn = WHITE;
                }
                if (e.key.code == sf::Keyboard::F) {
                    game->board->flipBoard();
                }
            }

        }

        game->gameWindow.clear();
        game->drawBoard();
        game->drawPossibleMoves();
        game->drawPieces();
        if (game->movingPiece != nullptr) {
            game->movingPiece->pieceSprite.setPosition(position.x - size / 2, position.y - size / 2);
            game->gameWindow.draw(game->movingPiece->pieceSprite);
        }
        game->gameWindow.display();

        if (game->isCheckmated(WHITE)) {
            std::cout << "BLACK WINS" << std::endl;
        }
        if (game->isCheckmated(BLACK)) {
            std::cout << "WHITE WINS" << std::endl;
        }

        if (game->isStalemated(WHITE) || game->isStalemated(BLACK)) {
            std::cout << "STALEMATE" << std::endl;
        }
    }

        return 0;
}