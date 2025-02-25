#include "BeginnerEngine.hpp"
#include <random>

Move BeginnerEngine::getBestMove(const ChessBoard& board) {
    std::vector<Move> possibleMoves;
    
    // Get all possible moves
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            ChessPiece* piece = board.getPieceAt(i, j);
            if (piece && piece->getColor() == BLACK) {  // AI plays black
                auto moves = piece->getValidMoves(board);
                for (const auto& pos : moves) {
                    // Create move and evaluate it
                    ChessBoard tempBoard = board;
                    // Simulate move on tempBoard
                    float score = alphaBeta(tempBoard, maxDepth, 
                                         -std::numeric_limits<float>::infinity(),
                                         std::numeric_limits<float>::infinity(), 
                                         false);
                    possibleMoves.emplace_back(i, j, pos.first, pos.second, score);
                }
            }
        }
    }
    
    // Beginner: Choose randomly from top 50% of moves
    std::sort(possibleMoves.begin(), possibleMoves.end(),
             [](const Move& a, const Move& b) { return a.score > b.score; });
             
    int moveRange = std::max(1, static_cast<int>(possibleMoves.size() / 2));
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, moveRange - 1);
    
    return possibleMoves[dis(gen)];
} 