#include "BeginnerEngine.hpp"

Move ExpertEngine::getBestMove(const ChessBoard& board) {
    std::vector<Move> possibleMoves;
    float bestScore = -std::numeric_limits<float>::infinity();
    Move bestMove(0, 0, 0, 0);
    
    // Get all possible moves
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            ChessPiece* piece = board.getPieceAt(i, j);
            if (piece && piece->getColor() == BLACK) {
                auto moves = piece->getValidMoves(board);
                for (const auto& pos : moves) {
                    ChessBoard tempBoard = board;
                    // Simulate move on tempBoard
                    float score = alphaBeta(tempBoard, maxDepth, 
                                         -std::numeric_limits<float>::infinity(),
                                         std::numeric_limits<float>::infinity(), 
                                         false);
                    
                    // Expert: Always choose the best move
                    if (score > bestScore) {
                        bestScore = score;
                        bestMove = Move(i, j, pos.first, pos.second, score);
                    }
                }
            }
        }
    }
    
    return bestMove;
} 