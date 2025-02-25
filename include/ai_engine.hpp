#pragma once
#include <vector>
#include "board.hpp"
#include "position_evaluator.hpp"

class AIEngine {
public:
    enum class Difficulty {
        BEGINNER,    // Elo ~1200
        INTERMEDIATE, // Elo ~1600
        ADVANCED,    // Elo ~2000
        MASTER      // Elo ~2400+
    };

    AIEngine();
    void setDifficulty(Difficulty level);
    Move getBestMove(const Board& board);
    float evaluatePosition(const Board& board);

private:
    Difficulty currentDifficulty;
    PositionEvaluator evaluator;
    
    // Minimax with alpha-beta pruning
    Move minimaxRoot(const Board& board, int depth);
    float minimax(const Board& board, int depth, float alpha, float beta, bool maximizing);
    
    // Opening book and endgame tablebase
    std::optional<Move> getBookMove(const Board& board);
    std::optional<Move> getTablebaseMove(const Board& board);
}; 