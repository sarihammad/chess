#include <SFML/Graphics.hpp>
#include "constants.cpp"
#include "Board.h"
#include "ChessGame.h"
#include "helpers.cpp"
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
                            if (game->board->getPieceAt(j, i) != nullptr && x == j && y == i && game->board->getColorAt(j, i) == game->currTurn) {
                                game->movingPiece = game->board->getPieceAt(x, y);
                                // game->gameWindow.draw(game->movingPiece)
                            }
                        }
                    }
                }
            }

            if (e.type == sf::Event::MouseButtonReleased) {
// if !ispromoting
                if (game->movingPiece != nullptr) {
                    if (game->currTurn == game->movingPiece->pieceColor && game->movingPiece->isValidMove(game->board, x, y) && !game->board->nextMoveIsChecked(game->movingPiece, x, y)) {
                    game->movingPiece->movePiece(game->board, x, y);
                    game->currTurn = getOtherColor(game->currTurn);
                    // setSoundFromMove(&soundBuffer, &moveSound, movingPieceX, movingPieceY, x, y);
                    // moveSound.play();
                    } else {
                        game->movingPiece->pieceSprite.setPosition(size * game->movingPiece->getPiecePosition(game->board).first, size * game->movingPiece->getPiecePosition(game->board).second);
                    }
                    game->movingPiece = nullptr;
                }
                

            }

            if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::R) {
                game->board->initializeBoard(game->board->bottomPlayer);
                game->currTurn = WHITE;
            }
                // if (isPromoting) {
                //     int promotionX;
                //     if (currTurn == WHITE) {
                //         for (int i = 0; i < 8; i++) {
                //             if (boardState[0][i] == whitePromotion) promotionX = i;
                //         }
                //     } else {
                //         for (int i = 0; i < 8; i++) {
                //             if (boardState[7][i] == blackPromotion) promotionX = i;
                //         }
                //     }
                //     if (currTurn == WHITE) {
                //         if (x == promotionX && y == 0 && position.x % (int) size < size / 2 && position.y % (int) size < size / 2) {
                //             boardState[0][promotionX] = whiteRook;  
                //             isPromoting = false;   
                //             currTurn = BLACK;                                                   
                //         } else if (x == promotionX && y == 0 && position.x % (int) size > size / 2 && position.y % (int) size < size / 2) {
                //             boardState[0][promotionX] = whiteQueen;
                //             isPromoting = false;
                //             currTurn = BLACK;                                                                               
                //         } else if (x == promotionX && y == 0 && position.x % (int) size < size / 2 && position.y % (int) size > size / 2) {
                //             boardState[0][promotionX] = whiteBishop;
                //             isPromoting = false;
                //             currTurn = BLACK;                                                   
                //         } else if (x == promotionX && y == 0 && position.x % (int) size > size / 2 && position.y % (int) size > size / 2) {
                //             boardState[0][promotionX] = whiteKnight;
                //             isPromoting = false;
                //             currTurn = BLACK;                                                                               
                //         }

                //     } else {
                //         if (x == promotionX && y == 7 && position.x % (int) size < size / 2 && position.y % (int) size < size / 2) {
                //             boardState[7][promotionX] = blackRook;
                //             isPromoting = false;
                //             currTurn = WHITE;
                //         } else if (x == promotionX && y == 7 && position.x % (int) size > size / 2 && position.y % (int) size < size / 2) {
                //             boardState[7][promotionX] = blackQueen;
                //             isPromoting = false;
                //             currTurn = WHITE;
                //         } else if (x == promotionX && y == 7 && position.x % (int) size < size / 2 && position.y % (int) size > size / 2) {
                //             boardState[7][promotionX] = blackBishop;
                //             isPromoting = false;
                //             currTurn = WHITE;
                //         } else if (x == promotionX && y == 7 && position.x % (int) size > size / 2 && position.y % (int) size > size / 2) {
                //             boardState[7][promotionX] = blackKnight;
                //             isPromoting = false;
                //             currTurn = WHITE;
                //         }

                //     }

                // }
        }

        game->gameWindow.clear();
        game->drawBoard();
        if (game->movingPiece != nullptr) {
            game->movingPiece->pieceSprite.setPosition(position.x - size / 2, position.y - size / 2);
            game->gameWindow.draw(game->movingPiece->pieceSprite);
            game->drawPossibleMoves();
        }
        game->drawPieces();

        game->gameWindow.display();

    }

        return 0;
}



            






        

        // std::cout << boardState[0][0] << std::endl;


        // sf::CircleShape possibleMove;
        // possibleMove.setRadius(size/5);
        // sf::Color possibleMoveColor(0, 0, 0, 50);
        // possibleMove.setFillColor(possibleMoveColor);
        // possibleMove.setOrigin(size/5, size/5);


        // sf::RectangleShape possibleCaptureMove;
        // possibleCaptureMove.setOrigin(size/2, size/2);
        // sf::Vector2f possibleCaptureMoveSize(size, size);
        // sf::Color possibleCaptureMoveColor(255, 0, 0, 255);
        // possibleCaptureMove.setSize(possibleCaptureMoveSize);
        // possibleCaptureMove.setFillColor(possibleCaptureMoveColor);


        // if (isMoving == true && !isPromoting) {
        //     // drawAllPossibleMoves(possibleMove);
        //     for (int i = 0; i < 8; i++) {
        //         for (int j = 0; j < 8; j++) {
        //             switch (movingPiece) {
        //             case whitePawn:
        //                 if (currTurn == WHITE && pawnIsValidMove(WHITE, movingPieceX, movingPieceY, j, i) && !nextMoveInCheck(movingPiece, movingPieceX, movingPieceY, j, i)) {
        //                     if (isCaptureMove(WHITE, j, i)) {
        //                         possibleCaptureMove.setPosition(j*size + size/2, i*size + size/2);
        //                         game->gameWindow.draw(possibleCaptureMove);
        //                     } else {
        //                         possibleMove.setPosition(j*size + size/2, i*size + size/2);
        //                         game->gameWindow.draw(possibleMove);
        //                     }
        //                 }
        //                 break;
        //             case whiteRook:
        //                 if (currTurn == WHITE && rookIsValidMove(WHITE, movingPieceX, movingPieceY, j, i) && !nextMoveInCheck(movingPiece, movingPieceX, movingPieceY, j, i)) {
        //                     if (isCaptureMove(WHITE, j, i)) {
        //                         possibleCaptureMove.setPosition(j*size + size/2, i*size + size/2);
        //                         game->gameWindow.draw(possibleCaptureMove);
        //                     } else {
        //                         possibleMove.setPosition(j*size + size/2, i*size + size/2);
        //                         game->gameWindow.draw(possibleMove);
        //                     }
        //                 }
        //                 break;
        //             case whiteKnight:
        //                 if (currTurn == WHITE && knightIsValidMove(WHITE, movingPieceX, movingPieceY, j, i) && !nextMoveInCheck(movingPiece, movingPieceX, movingPieceY, j, i)) {
        //                     if (isCaptureMove(WHITE, j, i)) {
        //                         possibleCaptureMove.setPosition(j*size + size/2, i*size + size/2);
        //                         game->gameWindow.draw(possibleCaptureMove);
        //                     } else {
        //                         possibleMove.setPosition(j*size + size/2, i*size + size/2);
        //                         game->gameWindow.draw(possibleMove);
        //                     }
        //                 }
        //                 break;
        //             case whiteBishop:
        //                 if (currTurn == WHITE && bishopIsValidMove(WHITE, movingPieceX, movingPieceY, j, i) && !nextMoveInCheck(movingPiece, movingPieceX, movingPieceY, j, i)) {
        //                     if (isCaptureMove(WHITE, j, i)) {
        //                         possibleCaptureMove.setPosition(j*size + size/2, i*size + size/2);
        //                         game->gameWindow.draw(possibleCaptureMove);
        //                     } else {
        //                         possibleMove.setPosition(j*size + size/2, i*size + size/2);
        //                         game->gameWindow.draw(possibleMove);
        //                     }
        //                 }
        //                 break;
        //             case whiteQueen:
        //                 if (currTurn == WHITE && queenIsValidMove(WHITE, movingPieceX, movingPieceY, j, i) && !nextMoveInCheck(movingPiece, movingPieceX, movingPieceY, j, i)) {
        //                     if (isCaptureMove(WHITE, j, i)) {
        //                         possibleCaptureMove.setPosition(j*size + size/2, i*size + size/2);
        //                         game->gameWindow.draw(possibleCaptureMove);
        //                     } else {
        //                         possibleMove.setPosition(j*size + size/2, i*size + size/2);
        //                         game->gameWindow.draw(possibleMove);
        //                     }
        //                 }
        //                 break;
        //             case whiteKing:
        //                 if (currTurn == WHITE && kingIsValidMove(WHITE, movingPieceX, movingPieceY, j, i) && !nextMoveInCheck(movingPiece, movingPieceX, movingPieceY, j, i)) {
        //                     if (isCaptureMove(WHITE, j, i)) {
        //                         possibleCaptureMove.setPosition(j*size + size/2, i*size + size/2);
        //                         game->gameWindow.draw(possibleCaptureMove);
        //                     } else {
        //                         possibleMove.setPosition(j*size + size/2, i*size + size/2);
        //                         game->gameWindow.draw(possibleMove);
        //                     }
        //                 }
        //                 break;
        //             case blackPawn:
        //                 if (currTurn == BLACK && pawnIsValidMove(BLACK, movingPieceX, movingPieceY, j, i) && !nextMoveInCheck(movingPiece, movingPieceX, movingPieceY, j, i)) {
        //                     if (isCaptureMove(BLACK, j, i)) {
        //                         possibleCaptureMove.setPosition(j*size + size/2, i*size + size/2);
        //                         game->gameWindow.draw(possibleCaptureMove);
        //                     } else {
        //                         possibleMove.setPosition(j*size + size/2, i*size + size/2);
        //                         game->gameWindow.draw(possibleMove);
        //                     }
        //                 }
        //                 break;
        //             case blackRook:
        //                 if (currTurn == BLACK && rookIsValidMove(BLACK, movingPieceX, movingPieceY, j, i) && !nextMoveInCheck(movingPiece, movingPieceX, movingPieceY, j, i)) {
        //                     if (isCaptureMove(BLACK, j, i)) {
        //                         possibleCaptureMove.setPosition(j*size + size/2, i*size + size/2);
        //                         game->gameWindow.draw(possibleCaptureMove);
        //                     } else {
        //                         possibleMove.setPosition(j*size + size/2, i*size + size/2);
        //                         game->gameWindow.draw(possibleMove);
        //                     }
        //                 }
        //                 break;
        //             case blackKnight:
        //                 if (currTurn == BLACK && knightIsValidMove(BLACK, movingPieceX, movingPieceY, j, i) && !nextMoveInCheck(movingPiece, movingPieceX, movingPieceY, j, i)) {
        //                     if (isCaptureMove(BLACK, j, i)) {
        //                         possibleCaptureMove.setPosition(j*size + size/2, i*size + size/2);
        //                         game->gameWindow.draw(possibleCaptureMove);
        //                     } else {
        //                         possibleMove.setPosition(j*size + size/2, i*size + size/2);
        //                         game->gameWindow.draw(possibleMove);
        //                     }                       
        //                 }
        //                 break;
        //             case blackBishop:
        //                 if (currTurn == BLACK && bishopIsValidMove(BLACK, movingPieceX, movingPieceY, j, i) && !nextMoveInCheck(movingPiece, movingPieceX, movingPieceY, j, i)) {
        //                     if (isCaptureMove(BLACK, j, i)) {
        //                         possibleCaptureMove.setPosition(j*size + size/2, i*size + size/2);
        //                         game->gameWindow.draw(possibleCaptureMove);
        //                     } else {
        //                         possibleMove.setPosition(j*size + size/2, i*size + size/2);
        //                         game->gameWindow.draw(possibleMove);
        //                     }
        //                 }
        //                 break;
        //             case blackQueen:
        //                 if (currTurn == BLACK && queenIsValidMove(BLACK, movingPieceX, movingPieceY, j, i) && !nextMoveInCheck(movingPiece, movingPieceX, movingPieceY, j, i)) {
        //                     if (isCaptureMove(BLACK, j, i)) {
        //                         possibleCaptureMove.setPosition(j*size + size/2, i*size + size/2);
        //                         game->gameWindow.draw(possibleCaptureMove);
        //                     } else {
        //                         possibleMove.setPosition(j*size + size/2, i*size + size/2);
        //                         game->gameWindow.draw(possibleMove);
        //                     }
        //                 }
        //                 break;
        //             case blackKing:
        //                 if (currTurn == BLACK && kingIsValidMove(BLACK, movingPieceX, movingPieceY, j, i) && !nextMoveInCheck(movingPiece, movingPieceX, movingPieceY, j, i)) {
        //                     if (isCaptureMove(BLACK, j, i)) {
        //                         possibleCaptureMove.setPosition(j*size + size/2, i*size + size/2);
        //                         game->gameWindow.draw(possibleCaptureMove);
        //                     } else {
        //                         possibleMove.setPosition(j*size + size/2, i*size + size/2);
        //                         game->gameWindow.draw(possibleMove);
        //                     }
        //                 }
        //                 break;
        //             }
        //         }
        //     }
        // }


        // for (int y = 0; y < 8; y++) {
        //     for (int x = 0; x < 8; x++) {
        //         if (board->getTypeAt(x, y) == "EMPTY") continue;
                
        //     } 
        // }

        // for (int i = 0; i < 8; i++) {
        //     for (int j = 0; j < 8; j++) {
        //         int piece = boardState[i][j];
        //         if (piece == 0) continue;

        //         switch (piece) {
        //         case whitePawn:
        //             currTurn == WHITE && isMoving && movingPieceX == j && movingPieceY == i && !isPromoting ? white_pawn.setPosition(position.x - size/2, position.y - size/2) : white_pawn.setPosition(j*size, i*size);
        //             game->gameWindow.draw(white_pawn);
        //             break;
        //         case whiteRook:
        //             currTurn == WHITE && isMoving && movingPieceX == j && movingPieceY == i && !isPromoting ? white_rook.setPosition(position.x - size/2, position.y - size/2) : white_rook.setPosition(j*size, i*size);
        //             game->gameWindow.draw(white_rook);
        //             break;
        //         case whiteKnight:
        //             currTurn == WHITE && isMoving && movingPieceX == j && movingPieceY == i && !isPromoting ? white_knight.setPosition(position.x - size/2, position.y - size/2) : white_knight.setPosition(j*size, i*size);
        //             game->gameWindow.draw(white_knight);
        //             break;
        //         case whiteBishop:
        //             currTurn == WHITE && isMoving && movingPieceX == j && movingPieceY == i && !isPromoting ? white_bishop.setPosition(position.x - size/2, position.y - size/2) : white_bishop.setPosition(j*size, i*size);
        //             game->gameWindow.draw(white_bishop);
        //             break;
        //         case whiteQueen:
        //             currTurn == WHITE && isMoving && movingPieceX == j && movingPieceY == i && !isPromoting ? white_queen.setPosition(position.x - size/2, position.y - size/2) : white_queen.setPosition(j*size, i*size);
        //             game->gameWindow.draw(white_queen);
        //             break;
        //         case whiteKing:
        //             currTurn == WHITE && isMoving && movingPieceX == j && movingPieceY == i && !isPromoting ? white_king.setPosition(position.x - size/2, position.y - size/2) : white_king.setPosition(j*size, i*size);
        //             game->gameWindow.draw(white_king);
        //             break;
        //         case whitePromotion:
        //             white_promotion.setPosition(j*size, i*size);
        //             game->gameWindow.draw(white_promotion);
        //             break;
        //         case blackPawn:
        //             currTurn == BLACK && isMoving && movingPieceX == j && movingPieceY == i && !isPromoting ? black_pawn.setPosition(position.x - size/2, position.y - size/2) : black_pawn.setPosition(j*size, i*size);
        //             game->gameWindow.draw(black_pawn);
        //             break;
        //         case blackRook:
        //             currTurn == BLACK && isMoving && movingPieceX == j && movingPieceY == i && !isPromoting ? black_rook.setPosition(position.x - size/2, position.y - size/2) : black_rook.setPosition(j*size, i*size);
        //             game->gameWindow.draw(black_rook);
        //             break;
        //         case blackKnight:
        //             currTurn == BLACK && isMoving && movingPieceX == j && movingPieceY == i && !isPromoting ? black_knight.setPosition(position.x - size/2, position.y - size/2) : black_knight.setPosition(j*size, i*size);
        //             game->gameWindow.draw(black_knight);
        //             break;
        //         case blackBishop:
        //             currTurn == BLACK && isMoving && movingPieceX == j && movingPieceY == i && !isPromoting ? black_bishop.setPosition(position.x - size/2, position.y - size/2) : black_bishop.setPosition(j*size, i*size);
        //             game->gameWindow.draw(black_bishop);
        //             break;
        //         case blackQueen:
        //             currTurn == BLACK && isMoving && movingPieceX == j && movingPieceY == i && !isPromoting ? black_queen.setPosition(position.x - size/2, position.y - size/2) : black_queen.setPosition(j*size, i*size);
        //             game->gameWindow.draw(black_queen);
        //             break;
        //         case blackKing:
        //             currTurn == BLACK && isMoving && movingPieceX == j && movingPieceY == i && !isPromoting ? black_king.setPosition(position.x - size/2, position.y - size/2) : black_king.setPosition(j*size, i*size);
        //             game->gameWindow.draw(black_king);
        //             break;
        //         case blackPromotion:
        //             black_promotion.setPosition(j*size, i*size);
        //             game->gameWindow.draw(black_promotion);
        //             break;
        //         }
        //     }
        // }

        // std::cout << whiteEnPassantActive << " " << blackEnPassantActive << std::endl;


        // if (inCheck(WHITE)) {
        //     if (inCheckmate(WHITE)) {
        //         std::cout << "BLACK WINS" << std::endl;
        //         // game->gameWindow.close();
        //     }
        // }
        // if (inCheck(BLACK)) {
        //     if (inCheckmate(BLACK)) {
        //         std::cout << "WHITE WINS" << std::endl;
        //         // game->gameWindow.close();
        //     }
        // }
        
        // if (inStalemate(WHITE) || inStalemate(BLACK)) std::cout << "STALEMATE" << std::endl;


