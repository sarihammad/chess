#pragma once

#include <memory>

class Game {
private:
    std::unique_ptr<ChessEngine> ai;
}; 