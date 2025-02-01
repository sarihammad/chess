#pragma once
#include <torch/torch.h>
#include "ChessBoard.h"

class ChessEngine {
protected:
    int maxDepth;
    torch::jit::script::Module model;
    
    // Evaluation functions
    float evaluatePosition(const ChessBoard& board);
    float alphaBeta(ChessBoard& board, int depth, float alpha, float beta, bool maximizing);
    
    // Neural network helpers
    torch::Tensor boardToTensor(const ChessBoard& board);
    std::vector<std::pair<int, int>> getPossibleMoves(const ChessBoard& board, PieceColor color);

    float getMoveScore(const ChessBoard& board, const Move& move);
    std::vector<Move> orderMoves(const ChessBoard& board, std::vector<Move>& moves);

    float evaluatePositionHeuristic(const ChessBoard& board);
    float getPieceValue(const ChessPiece* piece, int x, int y);

    struct PositionalFeatures {
        float pawnStructure;
        float kingProtection;
        float pieceActivity;
        float centerControl;
        float development;
        float rookPosition;
    };

    float evaluateKingSafety(const ChessBoard& board, PieceColor color);
    float evaluatePawnStructure(const ChessBoard& board, PieceColor color);
    float evaluatePieceActivity(const ChessBoard& board, PieceColor color);
    float evaluateDevelopment(const ChessBoard& board, PieceColor color);
    float evaluateRookPosition(const ChessBoard& board, PieceColor color);
    PositionalFeatures getPositionalFeatures(const ChessBoard& board, PieceColor color);

    bool canBeAttackedByPawn(const ChessBoard& board, int x, int y, PieceColor color);
    bool hasBishopPair(const ChessBoard& board, PieceColor color);
    bool isOpenFile(const ChessBoard& board, int file);
    float calculateGamePhase(const ChessBoard& board);

public:
    ChessEngine(const std::string& modelPath, int depth);
    virtual Move getBestMove(const ChessBoard& board) = 0;
}; 