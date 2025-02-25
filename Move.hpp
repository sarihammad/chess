#pragma once

struct Move {
    int fromX, fromY;
    int toX, toY;
    float score;  // Used for move ordering in alpha-beta

    Move(int fx, int fy, int tx, int ty, float s = 0.0f) 
        : fromX(fx), fromY(fy), toX(tx), toY(ty), score(s) {}
}; 