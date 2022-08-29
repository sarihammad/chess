#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include "board.h"
#include "knight.h"

#define whitePawn 1
#define whiteRook 2
#define whiteKnight 3
#define whiteBishop 4
#define whiteQueen 5
#define whiteKing 6
#define blackPawn -1
#define blackRook -2
#define blackKnight -3
#define blackBishop -4
#define blackQueen -5
#define blackKing -6

#define whitePromotion 7
#define blackPromotion -7


#define windowSize 800
#define squareSize (float) (windowSize / 8)
#define spriteScale (float) (windowSize / 800)

color currTurn = WHITE;

bool isMoving = false;
int movingPiece;
int movingPieceX;
int movingPieceY;

bool whiteKingMoved = false;
bool whiteHRookMoved = false;
bool whiteARookMoved = false;
bool blackKingMoved = false;
bool blackHRookMoved = false;
bool blackARookMoved = false;

bool whiteCanCastleKingside = false;
bool whiteCanCastleQueenside = false;
bool blackCanCastleKingside = false;
bool blackCanCastleQueenside = false;

bool whiteEnPassantActive = false;
int whiteEnPassantX;

bool blackEnPassantActive = false;
int blackEnPassantX;

bool isPromoting = false;

bool captured = false;


using namespace sf;



SoundBuffer soundBuffer;
Sound moveSound;


int boardState[8][8] =
{ 
    {-2,-3,-4,-5,-6,-4,-3,-2},
    {-1,-1,-1,-1,-1,-1,-1,-1},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {1, 1, 1, 1, 1, 1, 1, 1},
    {2, 3, 4, 5, 6, 4, 3, 2}
};



bool pawnIsValidMove(color color, int x, int y, int new_x, int new_y) {
    if (color == WHITE) {
        if (x - new_x == 0) {
            if (boardState[new_y][new_x] == 0 && (y - new_y == 1 || (y == 6 && y - new_y == 2 && boardState[5][new_x] == 0))) {
                return true;
            } else {
                return false;
            }
        } else if (abs(x - new_x) == 1 && y - new_y == 1 && boardState[new_y][new_x] < 0) {
            return true;
        } else if (whiteEnPassantActive && whiteEnPassantX == new_x && y == 3 && new_y == 2 && abs(x - new_x) == 1 && boardState[new_y][new_x] == 0 && boardState[new_y+1][new_x] == blackPawn) {
            return true;
        } else {
            return false;
        }
    } else {
        if (x - new_x == 0) {
            if (boardState[new_y][new_x] == 0 && (new_y - y == 1 || (y == 1 && new_y - y == 2 && boardState[2][new_x] == 0))) {
                return true;
            } else {
                return false;
            }
        } else if (abs(x - new_x) == 1 && new_y - y == 1 && boardState[new_y][new_x] > 0) {
            return true;
        } else if (blackEnPassantActive && blackEnPassantX == new_x && y == 4 && new_y == 5 && abs(x - new_x) == 1 && boardState[new_y][new_x] == 0 && boardState[new_y-1][new_x] == whitePawn) {
            return true;
        } else {
            return false;
        }
    } 
}

bool knightIsValidMove(color color, int x, int y, int new_x, int new_y) {
    // out of bounds
    if (x < 0 || x > 7 || y < 0 || y > 7 || new_x < 0 || new_x > 7 || new_y < 0 || new_y > 7) return false;

    // piece did not move
    if (x == new_x && y == new_y) return false;

    if (abs(new_x - x) == 2 && abs(new_y - y) == 1 || abs(new_x - x) == 1 && abs(new_y - y) == 2) {
        if (color == WHITE && boardState[new_y][new_x] <= 0) {
            return true;
        }
        if (color == BLACK && boardState[new_y][new_x] >= 0) {
            return true;
        }
    }

    return false;

}

bool rookIsValidMove(color color, int x, int y, int new_x, int new_y) {
    if (x < 0 || x > 7 || y < 0 || y > 7 || new_x < 0 || new_x > 7 || new_y < 0 || new_y > 7) return false;
    if (x == new_x && y == new_y) return false;
    if (x != new_x && y != new_y) return false;
    if (color == WHITE && boardState[new_y][new_x] > 0) return false;
    if (color == BLACK && boardState[new_y][new_x] < 0) return false;
    if (x == new_x) {
        for (int i = min(y, new_y) + 1; i < max(y, new_y); i++) {
            if (boardState[i][x] != 0) return false;
        }
        return true;
    } else {
        for (int j = min(x, new_x) + 1; j < max(x, new_x); j++) {
            if (boardState[y][j] != 0) return false;
        }
        return true;
    }
}

bool bishopIsValidMove(color color, int x, int y, int new_x, int new_y) {
    if (x < 0 || x > 7 || y < 0 || y > 7 || new_x < 0 || new_x > 7 || new_y < 0 || new_y > 7) return false;
    if (x == new_x && y == new_y) return false;
    if (abs(x - new_x) != abs(y - new_y)) return false;
    if (color == WHITE && boardState[new_y][new_x] > 0) return false;
    if (color == BLACK && boardState[new_y][new_x] < 0) return false;


    int currX = x < new_x ? x+1 : x-1;
    int currY = y < new_y ? y+1 : y-1;
    while (currX != new_x && currY != new_y) {
        if (boardState[currY][currX] != 0) return false;
        currX < new_x ? currX++ : currX--;
        currY < new_y ? currY++ : currY--;
    }

    // int tempBoardState[8][8];
    // memcpy(tempBoardState, boardState, sizeof(boardState));
    

     return true;

}

bool queenIsValidMove(color color, int x, int y, int new_x, int new_y) {
    return bishopIsValidMove(color, x, y, new_x, new_y) || rookIsValidMove(color, x, y, new_x, new_y); 
}

bool kingIsValidMove(color color, int x, int y, int new_x, int new_y) {
    if (x < 0 || x > 7 || y < 0 || y > 7 || new_x < 0 || new_x > 7 || new_y < 0 || new_y > 7) return false;
    if (x == new_x && y == new_y) return false;


    if (color == WHITE) {

        if (x == 4 && y == 7 && new_x == 2 && new_y == 7 && whiteCanCastleQueenside) {
            return true;
        }
        if (x == 4 && y == 7 && new_x == 6 && new_y == 7 && whiteCanCastleKingside) {
            return true;
        }   
    } else {
        if (!blackKingMoved) {
            if (x == 4 && y == 0 && new_x == 2 && new_y == 0 && blackCanCastleQueenside) {
                return true;
            }
            if (x == 4 && y == 0 && new_x == 6 && new_y == 0 && blackCanCastleKingside) {
                return true;
            }
        }
    }

    if (abs(x - new_x) != 1 || abs(y - new_y) != 1) {
        if (max(abs(x - new_x), abs(y - new_y)) != 1) return false;
    }
    if (color == WHITE && boardState[new_y][new_x] > 0) return false;
    if (color == BLACK && boardState[new_y][new_x] < 0) return false;


    return true;
}

bool inCheck(color color) {
    int kingX;
    int kingY;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (color == WHITE && boardState[i][j] == whiteKing) {
                kingX = j;
                kingY = i;
            }

            if (color == BLACK && boardState[i][j] == blackKing) {
                kingX = j;
                kingY = i;
            }
        }
    }

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (color == WHITE && boardState[i][j] < 0) {
                switch (boardState[i][j])
                {
                case blackPawn:
                    if (pawnIsValidMove(BLACK, j, i, kingX, kingY)) return true;
                    break;
                case blackRook:
                    if (rookIsValidMove(BLACK, j, i, kingX, kingY)) return true;
                    break;
                case blackKnight:
                    if (knightIsValidMove(BLACK, j, i, kingX, kingY)) return true;
                    break;
                case blackBishop:
                    if (bishopIsValidMove(BLACK, j, i, kingX, kingY)) return true;
                    break;
                case blackQueen:
                    if (queenIsValidMove(BLACK, j, i, kingX, kingY)) return true;
                    break;
                case blackKing:
                    if (kingIsValidMove(BLACK, j, i, kingX, kingY)) return true;
                    break;
                }
            }

            if (color == BLACK && boardState[i][j] > 0) {
                switch (boardState[i][j])
                {
                case whitePawn:
                    if (pawnIsValidMove(WHITE, j, i, kingX, kingY)) return true;
                    break;
                case whiteRook:
                    if (rookIsValidMove(WHITE, j, i, kingX, kingY)) return true;
                    break;
                case whiteKnight:
                    if (knightIsValidMove(WHITE, j, i, kingX, kingY)) return true;
                    break;
                case whiteBishop:
                    if (bishopIsValidMove(WHITE, j, i, kingX, kingY)) return true;
                    break;
                case whiteQueen:
                    if (queenIsValidMove(WHITE, j, i, kingX, kingY)) return true;
                    break;
                case whiteKing:
                    if (kingIsValidMove(WHITE, j, i, kingX, kingY)) return true;
                    break;
                }
            }
        }
    }

    return false; 
}



bool nextMoveInCheck(int movingPiece, int x, int y, int new_x, int new_y) {
    int newSquarePiece = boardState[new_y][new_x];
    boardState[new_y][new_x] = movingPiece;
    boardState[y][x] = 0;
    if (movingPiece > 0) {
        if (inCheck(WHITE)) {
            boardState[y][x] = movingPiece;
            boardState[new_y][new_x] = newSquarePiece;
            return true;
        }

    } else {
        if (inCheck(BLACK)) {
            boardState[y][x] = movingPiece;
            boardState[new_y][new_x] = newSquarePiece;
            return true;
        }
    }
    boardState[y][x] = movingPiece;
    boardState[new_y][new_x] = newSquarePiece;
    return false;

}

bool noValidMove(color color) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (color == WHITE && boardState[i][j] > 0) {
                switch (boardState[i][j])
                {
                case whitePawn:
                    for (int k = 0; k < 8; k++) {
                        for (int l = 0; l < 8; l++) {
                            if (pawnIsValidMove(WHITE, j, i, l, k) && !nextMoveInCheck(whitePawn, j, i, l, k)) return false;
                        }
                    }
                    break;
                case whiteRook:
                    for (int k = 0; k < 8; k++) {
                        for (int l = 0; l < 8; l++) {
                            if (rookIsValidMove(WHITE, j, i, l, k) && !nextMoveInCheck(whiteRook, j, i, l, k)) return false;
                        }
                    }
                    break;
                case whiteKnight:
                    for (int k = 0; k < 8; k++) {
                        for (int l = 0; l < 8; l++) {
                            if (knightIsValidMove(WHITE, j, i, l, k) && !nextMoveInCheck(whiteKnight, j, i, l, k)) return false;
                        }
                    }
                    break;
                case whiteBishop:
                    for (int k = 0; k < 8; k++) {
                        for (int l = 0; l < 8; l++) {
                            if (bishopIsValidMove(WHITE, j, i, l, k) && !nextMoveInCheck(whiteBishop, j, i, l, k)) return false;
                        }
                    }
                    break;
                case whiteQueen:
                    for (int k = 0; k < 8; k++) {
                        for (int l = 0; l < 8; l++) {
                            if (queenIsValidMove(WHITE, j, i, l, k) && !nextMoveInCheck(whiteQueen, j, i, l, k)) return false;
                        }
                    }
                    break;
                case whiteKing:
                    for (int k = 0; k < 8; k++) {
                        for (int l = 0; l < 8; l++) {
                            if (kingIsValidMove(WHITE, j, i, l, k) && !nextMoveInCheck(whiteKing, j, i, l, k)) return false;
                        }
                    }
                    break;
                }
            }

            if (color == BLACK && boardState[i][j] < 0) {
                switch (boardState[i][j])
                {
                case blackPawn:
                    for (int k = 0; k < 8; k++) {
                        for (int l = 0; l < 8; l++) {
                            if (pawnIsValidMove(BLACK, j, i, l, k) && !nextMoveInCheck(blackPawn, j, i, l, k)) return false;
                        }
                    }
                    break;
                case blackRook:
                    for (int k = 0; k < 8; k++) {
                        for (int l = 0; l < 8; l++) {
                            if (rookIsValidMove(BLACK, j, i, l, k) && !nextMoveInCheck(blackRook, j, i, l, k)) return false;
                        }
                    }
                    break;
                case blackKnight:
                    for (int k = 0; k < 8; k++) {
                        for (int l = 0; l < 8; l++) {
                            if (knightIsValidMove(BLACK, j, i, l, k) && !nextMoveInCheck(blackKnight, j, i, l, k)) return false;
                        }
                    }
                    break;
                case blackBishop:
                    for (int k = 0; k < 8; k++) {
                        for (int l = 0; l < 8; l++) {
                            if (bishopIsValidMove(BLACK, j, i, l, k) && !nextMoveInCheck(blackBishop, j, i, l, k)) return false;
                        }
                    }
                    break;
                case blackQueen:
                    for (int k = 0; k < 8; k++) {
                        for (int l = 0; l < 8; l++) {
                            if (queenIsValidMove(BLACK, j, i, l, k) && !nextMoveInCheck(blackQueen, j, i, l, k)) return false;
                        }
                    }
                    break;
                case blackKing:
                    for (int k = 0; k < 8; k++) {
                        for (int l = 0; l < 8; l++) {
                            if (kingIsValidMove(BLACK, j, i, l, k) && !nextMoveInCheck(blackKing, j, i, l, k)) return false;
                        }
                    }
                    break;
                }
            }
        }
    }

    return true;
}

bool inCheckmate(color color) {
    return noValidMove(color) && inCheck(color) ? true : false;
}

bool inStalemate(color color) {
    return noValidMove(color) && !inCheck(color) && currTurn == color ? true : false;
}



bool isCaptureMove(color color, int new_x, int new_y) {
    if (color == WHITE && boardState[new_y][new_x] < 0) return true;
    if (color == BLACK && boardState[new_y][new_x] > 0) return true;
    if (color == WHITE && whiteEnPassantActive && movingPiece == whitePawn && new_x == whiteEnPassantX && new_y == 2 && boardState[new_y][new_x] == 0) return true;
    if (color == BLACK && blackEnPassantActive && movingPiece == blackPawn && new_x == blackEnPassantX && new_y == 5 && boardState[new_y][new_x] == 0) return true;

    return false;
}

bool isCastlingMove(color color, int x, int y, int new_x, int new_y) {
    return kingIsValidMove(color, x, y, new_x, new_y) && abs(new_x - x) == 2 ? true : false;
}

void setSoundFromMove(SoundBuffer *soundBuffer, Sound *sound, int x, int y, int new_x, int new_y) {
    if (inCheckmate(BLACK)) {
        soundBuffer->loadFromFile("sounds/checkmate_win.wav");
    } else if (inCheckmate(WHITE)) {
        soundBuffer->loadFromFile("sounds/checkmate_loss.wav");
    } else if (inStalemate(WHITE) || inStalemate(BLACK)) {
        soundBuffer->loadFromFile("sounds/stalemate.wav");
    } else if (inCheck(WHITE) || inCheck(BLACK)) {
        soundBuffer->loadFromFile("sounds/check.wav");
    } else if (captured) {
        soundBuffer->loadFromFile("sounds/capture.wav");
    } else if (isCastlingMove(WHITE, x, y, new_x, new_y) || isCastlingMove(BLACK, x, y, new_x, new_y)) {
        soundBuffer->loadFromFile("sounds/castle.wav");
    } else {
        soundBuffer->loadFromFile("sounds/move.wav");
    }
    sound->setBuffer(*soundBuffer);
}



int main()
{
    RenderWindow game(VideoMode(windowSize, windowSize), "Chess", Style::Titlebar | Style::Close);
    // board board;
    Texture t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,t14,t15;
    t1.loadFromFile("images/board.png");
    t2.loadFromFile("images/white_pawn.png");
    t3.loadFromFile("images/white_rook.png");
    t4.loadFromFile("images/white_knight.png");
    t5.loadFromFile("images/white_bishop.png");
    t6.loadFromFile("images/white_king.png");
    t7.loadFromFile("images/white_queen.png");
    t8.loadFromFile("images/white_promotion.png");
    t9.loadFromFile("images/black_pawn.png");
    t10.loadFromFile("images/black_rook.png");
    t11.loadFromFile("images/black_knight.png");
    t12.loadFromFile("images/black_bishop.png");
    t13.loadFromFile("images/black_king.png");
    t14.loadFromFile("images/black_queen.png");
    t15.loadFromFile("images/black_promotion.png");

    
    Sprite chessboard(t1);
    chessboard.setScale(spriteScale, spriteScale);
    Sprite white_pawn(t2);
    white_pawn.setScale(spriteScale, spriteScale);
    Sprite white_rook(t3);
    white_rook.setScale(spriteScale, spriteScale);
    Sprite white_knight(t4);
    white_knight.setScale(spriteScale, spriteScale);
    Sprite white_bishop(t5);
    white_bishop.setScale(spriteScale, spriteScale);
    Sprite white_king(t6);
    white_king.setScale(spriteScale, spriteScale);
    Sprite white_queen(t7);
    white_queen.setScale(spriteScale, spriteScale);
    Sprite white_promotion(t8);
    white_promotion.setScale(spriteScale, spriteScale);
    Sprite black_pawn(t9);
    black_pawn.setScale(spriteScale, spriteScale);
    Sprite black_rook(t10);
    black_rook.setScale(spriteScale, spriteScale);
    Sprite black_knight(t11);
    black_knight.setScale(spriteScale, spriteScale);
    Sprite black_bishop(t12);
    black_bishop.setScale(spriteScale, spriteScale);
    Sprite black_king(t13);
    black_king.setScale(spriteScale, spriteScale);
    Sprite black_queen(t14);
    black_queen.setScale(spriteScale, spriteScale);
    Sprite black_promotion(t15);
    black_promotion.setScale(spriteScale, spriteScale);

    // SoundBuffer moveSoundBuffer;
    // SoundBuffer captureSoundBuffer;
    // SoundBuffer checkSoundBuffer;
    // SoundBuffer checkmateSoundBuffer;



    while (game.isOpen()) {
        // int maxSide = max(game.getSize().x, game.getSize().y);
        // game.setSize(Vector2u(maxSide, maxSide));
        Vector2i position = Mouse::getPosition(game);
        int x = position.x / squareSize;
        int y = position.y / squareSize;


        if (boardState[7][4] == 0) whiteKingMoved = true;
        if (boardState[7][0] == 0) whiteARookMoved = true;
        if (boardState[7][7] == 0) whiteHRookMoved = true;
        
        if (boardState[0][4] == 0) blackKingMoved = true;
        if (boardState[0][0] == 0) blackARookMoved = true;
        if (boardState[0][7] == 0) blackHRookMoved = true;

        if (!whiteKingMoved && !whiteARookMoved && boardState[7][1] == 0 && boardState[7][2] == 0 && boardState[7][3] == 0 && !inCheck(WHITE) && !nextMoveInCheck(whiteKing, 4, 7, 3, 7) && !nextMoveInCheck(whiteKing, 4, 7, 2, 7)) {
            whiteCanCastleQueenside = true;
        } else {
            whiteCanCastleQueenside = false;
        }
            
        

        if (!blackKingMoved && !blackARookMoved && boardState[0][1] == 0 && boardState[0][2] == 0 && boardState[0][3] == 0 && !inCheck(BLACK) && !nextMoveInCheck(blackKing, 4, 0, 3, 0) && !nextMoveInCheck(blackKing, 4, 0, 2, 0)) {
            blackCanCastleQueenside = true;    
        } else {
            blackCanCastleQueenside = false;
        }

        if (!whiteKingMoved && !whiteHRookMoved && boardState[7][5] == 0 && boardState[7][6] == 0 && !inCheck(WHITE) && !nextMoveInCheck(whiteKing, 4, 7, 5, 7) && !nextMoveInCheck(whiteKing, 4, 7, 6, 7)) {
            whiteCanCastleKingside = true;
        } else {
            whiteCanCastleKingside = false;
        }

        if (!blackKingMoved && !blackHRookMoved && boardState[0][5] == 0 && boardState[0][6] == 0 && !inCheck(BLACK) && !nextMoveInCheck(blackKing, 4, 0, 5, 0) && !nextMoveInCheck(blackKing, 4, 0, 6, 0)) {
            blackCanCastleKingside = true;
        } else {
            blackCanCastleKingside = false;
        }


        Event e;
        while (game.pollEvent(e)) {
            if (e.type == Event::Closed) 
                game.close();

            if(e.type == Event::MouseButtonPressed) {
                if (e.key.code == Mouse::Left) {
                    for (int i = 0; i < 8; i++) {
                        for (int j = 0; j < 8; j++) {
                            if (boardState[i][j] != 0 && x == j && y == i) {
                                isMoving = true;
                                movingPiece = boardState[i][j];
                                movingPieceX = j;
                                movingPieceY = i;
                            }
                        }
                    }

                if (isPromoting) {
                    int promotionX;
                    if (currTurn == WHITE) {
                        for (int i = 0; i < 8; i++) {
                            if (boardState[0][i] == whitePromotion) promotionX = i;
                        }
                    } else {
                        for (int i = 0; i < 8; i++) {
                            if (boardState[7][i] == blackPromotion) promotionX = i;
                        }
                    }
                    if (currTurn == WHITE) {
                        if (x == promotionX && y == 0 && position.x % (int) squareSize < squareSize / 2 && position.y % (int) squareSize < squareSize / 2) {
                            boardState[0][promotionX] = whiteRook;  
                            isPromoting = false;   
                            currTurn = BLACK;                                                   
                        } else if (x == promotionX && y == 0 && position.x % (int) squareSize > squareSize / 2 && position.y % (int) squareSize < squareSize / 2) {
                            boardState[0][promotionX] = whiteQueen;
                            isPromoting = false;
                            currTurn = BLACK;                                                                               
                        } else if (x == promotionX && y == 0 && position.x % (int) squareSize < squareSize / 2 && position.y % (int) squareSize > squareSize / 2) {
                            boardState[0][promotionX] = whiteBishop;
                            isPromoting = false;
                            currTurn = BLACK;                                                   
                        } else if (x == promotionX && y == 0 && position.x % (int) squareSize > squareSize / 2 && position.y % (int) squareSize > squareSize / 2) {
                            boardState[0][promotionX] = whiteKnight;
                            isPromoting = false;
                            currTurn = BLACK;                                                                               
                        }

                    } else {
                        if (x == promotionX && y == 7 && position.x % (int) squareSize < squareSize / 2 && position.y % (int) squareSize < squareSize / 2) {
                            boardState[7][promotionX] = blackRook;
                            isPromoting = false;
                            currTurn = WHITE;
                        } else if (x == promotionX && y == 7 && position.x % (int) squareSize > squareSize / 2 && position.y % (int) squareSize < squareSize / 2) {
                            boardState[7][promotionX] = blackQueen;
                            isPromoting = false;
                            currTurn = WHITE;
                        } else if (x == promotionX && y == 7 && position.x % (int) squareSize < squareSize / 2 && position.y % (int) squareSize > squareSize / 2) {
                            boardState[7][promotionX] = blackBishop;
                            isPromoting = false;
                            currTurn = WHITE;
                        } else if (x == promotionX && y == 7 && position.x % (int) squareSize > squareSize / 2 && position.y % (int) squareSize > squareSize / 2) {
                            boardState[7][promotionX] = blackKnight;
                            isPromoting = false;
                            currTurn = WHITE;
                        }

                    }

                }
                }
            }

            if (e.type == Event::MouseButtonReleased) {
                
                if (!isPromoting) {
                    switch (movingPiece) {
                    case whitePawn:
                        if (currTurn == WHITE && pawnIsValidMove(WHITE, movingPieceX, movingPieceY, x, y) && !nextMoveInCheck(whitePawn, movingPieceX, movingPieceY, x, y)) {
                            if (isCaptureMove(WHITE, x, y)) captured = true;
                            boardState[movingPieceY][movingPieceX] = 0;
                            if (boardState[y][x] == 0) {
                                boardState[(y) + 1][x] = 0;
                            }
                            if (y == 0) {
                                boardState[y][x] = whitePromotion;
                                isPromoting = true;

                            } else {
                                if (movingPieceY - (y) == 2) {
                                    blackEnPassantActive = true;
                                    blackEnPassantX = x;
                                }
                                boardState[y][x] = movingPiece;
                                currTurn = BLACK;
                            }
                            setSoundFromMove(&soundBuffer, &moveSound, movingPieceX, movingPieceY, x, y);
                            moveSound.play();
                            whiteEnPassantActive = false;
                            captured = false;
                        }
                        break;
                    case whiteRook:
                        if (currTurn == WHITE && rookIsValidMove(WHITE, movingPieceX, movingPieceY, x, y) && !nextMoveInCheck(whiteRook, movingPieceX, movingPieceY, x, y)) {
                            if (isCaptureMove(WHITE, x, y)) captured = true;
                            boardState[movingPieceY][movingPieceX] = 0;
                            boardState[y][x] = movingPiece;
                            currTurn = BLACK;
                            setSoundFromMove(&soundBuffer, &moveSound, movingPieceX, movingPieceY, x, y);
                            moveSound.play();
                            whiteEnPassantActive = false;
                            captured = false;
                        }
                        break;
                    case whiteKnight:
                        if (currTurn == WHITE && knightIsValidMove(WHITE, movingPieceX, movingPieceY, x, y) && !nextMoveInCheck(whiteKnight, movingPieceX, movingPieceY, x, y)) {
                            if (isCaptureMove(WHITE, x, y)) captured = true;
                            boardState[movingPieceY][movingPieceX] = 0;
                            boardState[y][x] = movingPiece; 
                            currTurn = BLACK;
                            setSoundFromMove(&soundBuffer, &moveSound, movingPieceX, movingPieceY, x, y);
                            moveSound.play();
                            whiteEnPassantActive = false;
                            captured = false;
                        }
                        break;
                    case whiteBishop:
                        if (currTurn == WHITE && bishopIsValidMove(WHITE, movingPieceX, movingPieceY, x, y) && !nextMoveInCheck(whiteBishop, movingPieceX, movingPieceY, x, y)) {
                            if (isCaptureMove(WHITE, x, y)) captured = true;
                            boardState[movingPieceY][movingPieceX] = 0;
                            boardState[y][x] = movingPiece; 
                            currTurn = BLACK;
                            setSoundFromMove(&soundBuffer, &moveSound, movingPieceX, movingPieceY, x, y);
                            moveSound.play();
                            whiteEnPassantActive = false;
                            captured = false;
                        }
                        break;
                    case whiteQueen:
                        if (currTurn == WHITE && queenIsValidMove(WHITE, movingPieceX, movingPieceY, x, y) && !nextMoveInCheck(whiteQueen, movingPieceX, movingPieceY, x, y)) {
                            if (isCaptureMove(WHITE, x, y)) captured = true;
                            boardState[movingPieceY][movingPieceX] = 0;
                            boardState[y][x] = movingPiece; 
                            currTurn = BLACK;
                            setSoundFromMove(&soundBuffer, &moveSound, movingPieceX, movingPieceY, x, y);
                            moveSound.play();
                            whiteEnPassantActive = false;
                            captured = false;
                        }
                        break;
                    case whiteKing:
                        if (currTurn == WHITE && kingIsValidMove(WHITE, movingPieceX, movingPieceY, x, y) && !nextMoveInCheck(whiteKing, movingPieceX, movingPieceY, x, y)) {
                            if (isCaptureMove(WHITE, x, y)) captured = true;
                            boardState[movingPieceY][movingPieceX] = 0;
                            boardState[y][x] = movingPiece;
                            if (whiteCanCastleQueenside && x == 2 && y == 7) {
                                boardState[7][0] = 0;
                                boardState[7][3] = whiteRook; 
                            }
                            if (whiteCanCastleKingside && x == 6 && y == 7) {
                                boardState[7][7] = 0;
                                boardState[7][5] = whiteRook; 
                            }
                            currTurn = BLACK;
                            setSoundFromMove(&soundBuffer, &moveSound, movingPieceX, movingPieceY, x, y);
                            moveSound.play();
                            whiteEnPassantActive = false;
                            captured = false;
                        }
                        break;
                    case blackPawn:
                        if (currTurn == BLACK && pawnIsValidMove(BLACK, movingPieceX, movingPieceY, x, y) && !nextMoveInCheck(blackPawn, movingPieceX, movingPieceY, x, y)) {
                            if (isCaptureMove(BLACK, x, y)) captured = true;
                            boardState[movingPieceY][movingPieceX] = 0;
                            if (boardState[y][x] == 0) {
                                boardState[(y) - 1][x] = 0;
                            }
                            if ((y) == 7) {
                                boardState[y][x] = blackPromotion;
                                isPromoting = true;

                            } else {
                                if ((y) - movingPieceY == 2) {
                                    whiteEnPassantActive = true;
                                    whiteEnPassantX = x;
                                }
                                boardState[y][x] = movingPiece;
                                currTurn = WHITE;
                            }
                            setSoundFromMove(&soundBuffer, &moveSound, movingPieceX, movingPieceY, x, y);
                            moveSound.play();
                            blackEnPassantActive = false;
                            captured = false;

                        }
                        break;
                    case blackRook:
                        if (currTurn == BLACK && rookIsValidMove(BLACK, movingPieceX, movingPieceY, x, y) && !nextMoveInCheck(blackRook, movingPieceX, movingPieceY, x, y)) {
                            if (isCaptureMove(BLACK, x, y)) captured = true;
                            boardState[movingPieceY][movingPieceX] = 0;
                            boardState[y][x] = movingPiece;
                            // if (movingPieceX == 0 && movingPieceY == 7 && !blackHRookMoved) blackHRookMoved = true;
                            // if (movingPieceX == 0 && movingPieceY == 0 && !blackARookMoved) blackARookMoved = true;  
                            currTurn = WHITE;
                            setSoundFromMove(&soundBuffer, &moveSound, movingPieceX, movingPieceY, x, y);
                            moveSound.play();
                            blackEnPassantActive = false;
                            captured = false;

                        }
                        break;
                    case blackKnight:
                        if (currTurn == BLACK && knightIsValidMove(BLACK, movingPieceX, movingPieceY, x, y) && !nextMoveInCheck(blackKnight, movingPieceX, movingPieceY, x, y)) {
                            if (isCaptureMove(BLACK, x, y)) captured = true;
                            boardState[movingPieceY][movingPieceX] = 0;
                            boardState[y][x] = movingPiece; 
                            setSoundFromMove(&soundBuffer, &moveSound, movingPieceX, movingPieceY, x, y);
                            moveSound.play();
                            currTurn = WHITE;
                            blackEnPassantActive = false;
                            captured = false;

                        }
                        break;
                    case blackBishop:
                        if (currTurn == BLACK && bishopIsValidMove(BLACK, movingPieceX, movingPieceY, x, y) && !nextMoveInCheck(blackBishop, movingPieceX, movingPieceY, x, y)) {
                            if (isCaptureMove(BLACK, x, y)) captured = true;
                            boardState[movingPieceY][movingPieceX] = 0;
                            boardState[y][x] = movingPiece; 
                            currTurn = WHITE;
                            setSoundFromMove(&soundBuffer, &moveSound, movingPieceX, movingPieceY, x, y);
                            moveSound.play();
                            blackEnPassantActive = false;
                            captured = false;

                        }
                        break;
                    case blackQueen:
                        if (currTurn == BLACK && queenIsValidMove(BLACK, movingPieceX, movingPieceY, x, y) && !nextMoveInCheck(blackQueen, movingPieceX, movingPieceY, x, y)) {
                            if (isCaptureMove(BLACK, x, y)) captured = true;
                            boardState[movingPieceY][movingPieceX] = 0;
                            boardState[y][x] = movingPiece; 
                            currTurn = WHITE;
                            setSoundFromMove(&soundBuffer, &moveSound, movingPieceX, movingPieceY, x, y);
                            moveSound.play();
                            blackEnPassantActive = false;
                            captured = false;

                        }
                        break;
                    case blackKing:
                        if (currTurn == BLACK && kingIsValidMove(BLACK, movingPieceX, movingPieceY, x, y) && !nextMoveInCheck(blackKing, movingPieceX, movingPieceY, x, y)) {
                            if (isCaptureMove(BLACK, x, y)) captured = true;
                            boardState[movingPieceY][movingPieceX] = 0;
                            boardState[y][x] = movingPiece;
                            if (blackCanCastleQueenside && x == 2 && y == 0) {
                                boardState[0][0] = 0;
                                boardState[0][3] = blackRook; 
                            }
                            if (blackCanCastleKingside && x == 6 && y == 0) {
                                boardState[0][7] = 0;
                                boardState[0][5] = blackRook; 
                            }
                            currTurn = WHITE;
                            setSoundFromMove(&soundBuffer, &moveSound, movingPieceX, movingPieceY, x, y);
                            moveSound.play();
                            blackEnPassantActive = false;
                            captured = false;
                        }
                        break;
                    }
                }


                isMoving = false;
            }



            

            if (e.type == Event::KeyPressed && e.key.code == Keyboard::R) {
                int initialState[8][8] = { 
                    {-2,-3,-4,-5,-6,-4,-3,-2},
                    {-1,-1,-1,-1,-1,-1,-1,-1},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {0, 0, 0, 0, 0, 0, 0, 0},
                    {1, 1, 1, 1, 1, 1, 1, 1},
                    {2, 3, 4, 5, 6, 4, 3, 2}
                };

                memcpy(boardState, initialState, sizeof(boardState));
                currTurn = WHITE;

                whiteKingMoved = false;
                whiteHRookMoved = false;
                whiteARookMoved = false;
                blackKingMoved = false;
                blackHRookMoved = false;
                blackARookMoved = false;

                whiteCanCastleKingside = false;
                whiteCanCastleQueenside = false;
                blackCanCastleKingside = false;
                blackCanCastleQueenside = false;

                whiteEnPassantActive = false;
                blackEnPassantActive = false;
            }
       
        }

        


        

        // std::cout << boardState[0][0] << std::endl;
        game.clear();
        game.draw(chessboard);

        CircleShape possibleMove;
        possibleMove.setRadius(squareSize/5);
        Color possibleMoveColor(0, 0, 0, 50);
        possibleMove.setFillColor(possibleMoveColor);
        possibleMove.setOrigin(squareSize/5, squareSize/5);


        RectangleShape possibleCaptureMove;
        possibleCaptureMove.setOrigin(squareSize/2, squareSize/2);
        Vector2f possibleCaptureMoveSize(squareSize, squareSize);
        Color possibleCaptureMoveColor(255, 0, 0, 255);
        possibleCaptureMove.setSize(possibleCaptureMoveSize);
        possibleCaptureMove.setFillColor(possibleCaptureMoveColor);


        if (isMoving == true && !isPromoting) {
            // drawAllPossibleMoves(possibleMove);
            for (int i = 0; i < 8; i++) {
                for (int j = 0; j < 8; j++) {
                    switch (movingPiece) {
                    case whitePawn:
                        if (currTurn == WHITE && pawnIsValidMove(WHITE, movingPieceX, movingPieceY, j, i) && !nextMoveInCheck(movingPiece, movingPieceX, movingPieceY, j, i)) {
                            if (isCaptureMove(WHITE, j, i)) {
                                possibleCaptureMove.setPosition(j*squareSize + squareSize/2, i*squareSize + squareSize/2);
                                game.draw(possibleCaptureMove);
                            } else {
                                possibleMove.setPosition(j*squareSize + squareSize/2, i*squareSize + squareSize/2);
                                game.draw(possibleMove);
                            }
                        }
                        break;
                    case whiteRook:
                        if (currTurn == WHITE && rookIsValidMove(WHITE, movingPieceX, movingPieceY, j, i) && !nextMoveInCheck(movingPiece, movingPieceX, movingPieceY, j, i)) {
                            if (isCaptureMove(WHITE, j, i)) {
                                possibleCaptureMove.setPosition(j*squareSize + squareSize/2, i*squareSize + squareSize/2);
                                game.draw(possibleCaptureMove);
                            } else {
                                possibleMove.setPosition(j*squareSize + squareSize/2, i*squareSize + squareSize/2);
                                game.draw(possibleMove);
                            }
                        }
                        break;
                    case whiteKnight:
                        if (currTurn == WHITE && knightIsValidMove(WHITE, movingPieceX, movingPieceY, j, i) && !nextMoveInCheck(movingPiece, movingPieceX, movingPieceY, j, i)) {
                            if (isCaptureMove(WHITE, j, i)) {
                                possibleCaptureMove.setPosition(j*squareSize + squareSize/2, i*squareSize + squareSize/2);
                                game.draw(possibleCaptureMove);
                            } else {
                                possibleMove.setPosition(j*squareSize + squareSize/2, i*squareSize + squareSize/2);
                                game.draw(possibleMove);
                            }
                        }
                        break;
                    case whiteBishop:
                        if (currTurn == WHITE && bishopIsValidMove(WHITE, movingPieceX, movingPieceY, j, i) && !nextMoveInCheck(movingPiece, movingPieceX, movingPieceY, j, i)) {
                            if (isCaptureMove(WHITE, j, i)) {
                                possibleCaptureMove.setPosition(j*squareSize + squareSize/2, i*squareSize + squareSize/2);
                                game.draw(possibleCaptureMove);
                            } else {
                                possibleMove.setPosition(j*squareSize + squareSize/2, i*squareSize + squareSize/2);
                                game.draw(possibleMove);
                            }
                        }
                        break;
                    case whiteQueen:
                        if (currTurn == WHITE && queenIsValidMove(WHITE, movingPieceX, movingPieceY, j, i) && !nextMoveInCheck(movingPiece, movingPieceX, movingPieceY, j, i)) {
                            if (isCaptureMove(WHITE, j, i)) {
                                possibleCaptureMove.setPosition(j*squareSize + squareSize/2, i*squareSize + squareSize/2);
                                game.draw(possibleCaptureMove);
                            } else {
                                possibleMove.setPosition(j*squareSize + squareSize/2, i*squareSize + squareSize/2);
                                game.draw(possibleMove);
                            }
                        }
                        break;
                    case whiteKing:
                        if (currTurn == WHITE && kingIsValidMove(WHITE, movingPieceX, movingPieceY, j, i) && !nextMoveInCheck(movingPiece, movingPieceX, movingPieceY, j, i)) {
                            if (isCaptureMove(WHITE, j, i)) {
                                possibleCaptureMove.setPosition(j*squareSize + squareSize/2, i*squareSize + squareSize/2);
                                game.draw(possibleCaptureMove);
                            } else {
                                possibleMove.setPosition(j*squareSize + squareSize/2, i*squareSize + squareSize/2);
                                game.draw(possibleMove);
                            }
                        }
                        break;
                    case blackPawn:
                        if (currTurn == BLACK && pawnIsValidMove(BLACK, movingPieceX, movingPieceY, j, i) && !nextMoveInCheck(movingPiece, movingPieceX, movingPieceY, j, i)) {
                            if (isCaptureMove(BLACK, j, i)) {
                                possibleCaptureMove.setPosition(j*squareSize + squareSize/2, i*squareSize + squareSize/2);
                                game.draw(possibleCaptureMove);
                            } else {
                                possibleMove.setPosition(j*squareSize + squareSize/2, i*squareSize + squareSize/2);
                                game.draw(possibleMove);
                            }
                        }
                        break;
                    case blackRook:
                        if (currTurn == BLACK && rookIsValidMove(BLACK, movingPieceX, movingPieceY, j, i) && !nextMoveInCheck(movingPiece, movingPieceX, movingPieceY, j, i)) {
                            if (isCaptureMove(BLACK, j, i)) {
                                possibleCaptureMove.setPosition(j*squareSize + squareSize/2, i*squareSize + squareSize/2);
                                game.draw(possibleCaptureMove);
                            } else {
                                possibleMove.setPosition(j*squareSize + squareSize/2, i*squareSize + squareSize/2);
                                game.draw(possibleMove);
                            }
                        }
                        break;
                    case blackKnight:
                        if (currTurn == BLACK && knightIsValidMove(BLACK, movingPieceX, movingPieceY, j, i) && !nextMoveInCheck(movingPiece, movingPieceX, movingPieceY, j, i)) {
                            if (isCaptureMove(BLACK, j, i)) {
                                possibleCaptureMove.setPosition(j*squareSize + squareSize/2, i*squareSize + squareSize/2);
                                game.draw(possibleCaptureMove);
                            } else {
                                possibleMove.setPosition(j*squareSize + squareSize/2, i*squareSize + squareSize/2);
                                game.draw(possibleMove);
                            }                       
                        }
                        break;
                    case blackBishop:
                        if (currTurn == BLACK && bishopIsValidMove(BLACK, movingPieceX, movingPieceY, j, i) && !nextMoveInCheck(movingPiece, movingPieceX, movingPieceY, j, i)) {
                            if (isCaptureMove(BLACK, j, i)) {
                                possibleCaptureMove.setPosition(j*squareSize + squareSize/2, i*squareSize + squareSize/2);
                                game.draw(possibleCaptureMove);
                            } else {
                                possibleMove.setPosition(j*squareSize + squareSize/2, i*squareSize + squareSize/2);
                                game.draw(possibleMove);
                            }
                        }
                        break;
                    case blackQueen:
                        if (currTurn == BLACK && queenIsValidMove(BLACK, movingPieceX, movingPieceY, j, i) && !nextMoveInCheck(movingPiece, movingPieceX, movingPieceY, j, i)) {
                            if (isCaptureMove(BLACK, j, i)) {
                                possibleCaptureMove.setPosition(j*squareSize + squareSize/2, i*squareSize + squareSize/2);
                                game.draw(possibleCaptureMove);
                            } else {
                                possibleMove.setPosition(j*squareSize + squareSize/2, i*squareSize + squareSize/2);
                                game.draw(possibleMove);
                            }
                        }
                        break;
                    case blackKing:
                        if (currTurn == BLACK && kingIsValidMove(BLACK, movingPieceX, movingPieceY, j, i) && !nextMoveInCheck(movingPiece, movingPieceX, movingPieceY, j, i)) {
                            if (isCaptureMove(BLACK, j, i)) {
                                possibleCaptureMove.setPosition(j*squareSize + squareSize/2, i*squareSize + squareSize/2);
                                game.draw(possibleCaptureMove);
                            } else {
                                possibleMove.setPosition(j*squareSize + squareSize/2, i*squareSize + squareSize/2);
                                game.draw(possibleMove);
                            }
                        }
                        break;
                    }
                }
            }
        }

        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                int piece = boardState[i][j];
                if (piece == 0) continue;

                switch (piece) {
                case whitePawn:
                    currTurn == WHITE && isMoving && movingPieceX == j && movingPieceY == i && !isPromoting ? white_pawn.setPosition(position.x - squareSize/2, position.y - squareSize/2) : white_pawn.setPosition(j*squareSize, i*squareSize);
                    game.draw(white_pawn);
                    break;
                case whiteRook:
                    currTurn == WHITE && isMoving && movingPieceX == j && movingPieceY == i && !isPromoting ? white_rook.setPosition(position.x - squareSize/2, position.y - squareSize/2) : white_rook.setPosition(j*squareSize, i*squareSize);
                    game.draw(white_rook);
                    break;
                case whiteKnight:
                    currTurn == WHITE && isMoving && movingPieceX == j && movingPieceY == i && !isPromoting ? white_knight.setPosition(position.x - squareSize/2, position.y - squareSize/2) : white_knight.setPosition(j*squareSize, i*squareSize);
                    game.draw(white_knight);
                    break;
                case whiteBishop:
                    currTurn == WHITE && isMoving && movingPieceX == j && movingPieceY == i && !isPromoting ? white_bishop.setPosition(position.x - squareSize/2, position.y - squareSize/2) : white_bishop.setPosition(j*squareSize, i*squareSize);
                    game.draw(white_bishop);
                    break;
                case whiteQueen:
                    currTurn == WHITE && isMoving && movingPieceX == j && movingPieceY == i && !isPromoting ? white_queen.setPosition(position.x - squareSize/2, position.y - squareSize/2) : white_queen.setPosition(j*squareSize, i*squareSize);
                    game.draw(white_queen);
                    break;
                case whiteKing:
                    currTurn == WHITE && isMoving && movingPieceX == j && movingPieceY == i && !isPromoting ? white_king.setPosition(position.x - squareSize/2, position.y - squareSize/2) : white_king.setPosition(j*squareSize, i*squareSize);
                    game.draw(white_king);
                    break;
                case whitePromotion:
                    white_promotion.setPosition(j*squareSize, i*squareSize);
                    game.draw(white_promotion);
                    break;
                case blackPawn:
                    currTurn == BLACK && isMoving && movingPieceX == j && movingPieceY == i && !isPromoting ? black_pawn.setPosition(position.x - squareSize/2, position.y - squareSize/2) : black_pawn.setPosition(j*squareSize, i*squareSize);
                    game.draw(black_pawn);
                    break;
                case blackRook:
                    currTurn == BLACK && isMoving && movingPieceX == j && movingPieceY == i && !isPromoting ? black_rook.setPosition(position.x - squareSize/2, position.y - squareSize/2) : black_rook.setPosition(j*squareSize, i*squareSize);
                    game.draw(black_rook);
                    break;
                case blackKnight:
                    currTurn == BLACK && isMoving && movingPieceX == j && movingPieceY == i && !isPromoting ? black_knight.setPosition(position.x - squareSize/2, position.y - squareSize/2) : black_knight.setPosition(j*squareSize, i*squareSize);
                    game.draw(black_knight);
                    break;
                case blackBishop:
                    currTurn == BLACK && isMoving && movingPieceX == j && movingPieceY == i && !isPromoting ? black_bishop.setPosition(position.x - squareSize/2, position.y - squareSize/2) : black_bishop.setPosition(j*squareSize, i*squareSize);
                    game.draw(black_bishop);
                    break;
                case blackQueen:
                    currTurn == BLACK && isMoving && movingPieceX == j && movingPieceY == i && !isPromoting ? black_queen.setPosition(position.x - squareSize/2, position.y - squareSize/2) : black_queen.setPosition(j*squareSize, i*squareSize);
                    game.draw(black_queen);
                    break;
                case blackKing:
                    currTurn == BLACK && isMoving && movingPieceX == j && movingPieceY == i && !isPromoting ? black_king.setPosition(position.x - squareSize/2, position.y - squareSize/2) : black_king.setPosition(j*squareSize, i*squareSize);
                    game.draw(black_king);
                    break;
                case blackPromotion:
                    black_promotion.setPosition(j*squareSize, i*squareSize);
                    game.draw(black_promotion);
                    break;
                }
            }
        }

        // std::cout << whiteEnPassantActive << " " << blackEnPassantActive << std::endl;

        game.display();

        if (inCheck(WHITE)) {
            if (inCheckmate(WHITE)) {
                std::cout << "BLACK WINS" << std::endl;
                // game.close();
            }
        }
        if (inCheck(BLACK)) {
            if (inCheckmate(BLACK)) {
                std::cout << "WHITE WINS" << std::endl;
                // game.close();
            }
        }
        
        if (inStalemate(WHITE) || inStalemate(BLACK)) std::cout << "STALEMATE" << std::endl;

    }
    return 0;
}
