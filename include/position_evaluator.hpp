#pragma once
#include <torch/torch.h>
#include "board.hpp"

class PositionEvaluator {
public:
    PositionEvaluator();
    float evaluatePosition(const Board& board);
    
private:
    torch::jit::script::Module model;
    
    // Basic evaluation parameters
    const float PAWN_VALUE = 1.0f;
    const float KNIGHT_VALUE = 3.0f;
    const float BISHOP_VALUE = 3.0f;
    const float ROOK_VALUE = 5.0f;
    const float QUEEN_VALUE = 9.0f;
    
    // Position evaluation helpers
    float evaluateMaterial(const Board& board);
    float evaluatePosition(const Board& board, Color color);
    float evaluatePawnStructure(const Board& board, Color color);
    float evaluateKingSafety(const Board& board, Color color);
    
    // CNN evaluation
    torch::Tensor boardToTensor(const Board& board);
    float evaluateWithCNN(const Board& board);
}; 