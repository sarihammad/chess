#pragma once

#include "../Color.h"

static Color getOtherColor(Color color) {
    if (color == WHITE) return BLACK;
    if (color == BLACK) return WHITE;
    return NONE;
}


static std::string colorToString(Color color) {
    if (color == WHITE) return "WHITE";
    if (color == BLACK) return "BLACK";
    return "NONE";
}