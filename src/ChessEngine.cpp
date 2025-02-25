#include "ChessEngine.hpp"
#include <limits>

ChessEngine::ChessEngine(const std::string& modelPath, int depth) : maxDepth(depth) {
    try {
        // Load the TorchScript model
        model = torch::jit::load(modelPath);
        model.eval();  // Set to evaluation mode
    } catch (const c10::Error& e) {
        std::cerr << "Error loading model: " << e.msg() << std::endl;
    }
}

float ChessEngine::evaluatePosition(const ChessBoard& board) {
    float nnScore = 0.0f;
    
    // Get neural network evaluation
    try {
        torch::Tensor boardTensor = boardToTensor(board);
        boardTensor = boardTensor.unsqueeze(0).to(torch::kFloat32);
        
        torch::NoGradGuard no_grad;
        std::vector<torch::jit::IValue> inputs;
        inputs.push_back(boardTensor);
        
        auto output = model.forward(inputs).toTensor();
        nnScore = output.item<float>();
    } catch (const std::exception& e) {
        std::cerr << "Neural network evaluation failed: " << e.what() << std::endl;
    }
    
    // Combine with heuristic evaluation
    float heuristicScore = evaluatePositionHeuristic(board);
    return 0.7f * nnScore + 0.3f * heuristicScore;  // Weighted combination
}

float ChessEngine::evaluatePositionHeuristic(const ChessBoard& board) {
    float whiteScore = 0.0f;
    float blackScore = 0.0f;
    
    // Piece-square tables for positional scoring
    const float pawnTable[8][8] = {
        {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
        {5.0, 5.0, 5.0, 5.0, 5.0, 5.0, 5.0, 5.0},
        {1.0, 1.0, 2.0, 3.0, 3.0, 2.0, 1.0, 1.0},
        {0.5, 0.5, 1.0, 2.5, 2.5, 1.0, 0.5, 0.5},
        {0.0, 0.0, 0.0, 2.0, 2.0, 0.0, 0.0, 0.0},
        {0.5,-0.5,-1.0, 0.0, 0.0,-1.0,-0.5, 0.5},
        {0.5, 1.0, 1.0,-2.0,-2.0, 1.0, 1.0, 0.5},
        {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}
    };
    
    // Evaluate each piece
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            ChessPiece* piece = board.getPieceAt(i, j);
            if (piece) {
                float value = getPieceValue(piece, i, j);
                if (piece->getColor() == WHITE) {
                    whiteScore += value;
                } else {
                    blackScore += value;
                }
                
                // Add positional bonus for pawns
                if (piece->getType() == PAWN) {
                    float posScore = pawnTable[piece->getColor() == WHITE ? i : 7-i][j];
                    if (piece->getColor() == WHITE) {
                        whiteScore += posScore;
                    } else {
                        blackScore += posScore;
                    }
                }
            }
        }
    }
    
    // Add sophisticated positional features
    whiteScore += evaluateKingSafety(board, WHITE) * 1.5f;      // King safety is important
    whiteScore += evaluatePawnStructure(board, WHITE) * 1.0f;   // Pawn structure is key
    whiteScore += evaluatePieceActivity(board, WHITE) * 1.2f;   // Active pieces are valuable
    
    blackScore += evaluateKingSafety(board, BLACK) * 1.5f;
    blackScore += evaluatePawnStructure(board, BLACK) * 1.0f;
    blackScore += evaluatePieceActivity(board, BLACK) * 1.2f;
    
    // Game phase adjustment
    float gamePhase = calculateGamePhase(board);
    float middleGameWeight = gamePhase;
    float endGameWeight = 1.0f - gamePhase;
    
    float finalScore = (whiteScore - blackScore) * middleGameWeight +
                      (whiteScore - blackScore) * endGameWeight * 1.2f;  // Endgame positions matter more
                      
    return finalScore;
}

float ChessEngine::getPieceValue(const ChessPiece* piece, int x, int y) {
    // Base piece values
    const float baseValues[] = {1.0, 3.0, 3.2, 5.0, 9.0, 0.0};  // pawn, knight, bishop, rook, queen, king
    float value = baseValues[piece->getType()];
    
    // Mobility bonus
    float mobilityBonus = piece->getValidMoves(board).size() * 0.1;
    
    // Center control bonus
    float centerBonus = 0.0;
    if (x >= 2 && x <= 5 && y >= 2 && y <= 5) {
        centerBonus = 0.2;
    }
    
    return value + mobilityBonus + centerBonus;
}

float ChessEngine::getMoveScore(const ChessBoard& board, const Move& move) {
    float score = 0.0f;
    
    ChessPiece* piece = board.getPieceAt(move.fromX, move.fromY);
    ChessPiece* capturedPiece = board.getPieceAt(move.toX, move.toY);
    
    // Piece values
    const float pieceValues[] = {1, 3, 3, 5, 9, 0};  // pawn, knight, bishop, rook, queen, king
    
    // Capturing moves
    if (capturedPiece) {
        score += 10 * pieceValues[capturedPiece->getType()];
    }
    
    // Center control for pawns and knights
    if (piece->getType() == PAWN || piece->getType() == KNIGHT) {
        int centerDist = std::abs(move.toX - 3.5) + std::abs(move.toY - 3.5);
        score += (4 - centerDist) * 0.1;
    }
    
    // Pawn advancement
    if (piece->getType() == PAWN) {
        score += (piece->getColor() == WHITE ? move.toY : 7 - move.toY) * 0.1;
    }
    
    return score;
}

std::vector<Move> ChessEngine::orderMoves(const ChessBoard& board, std::vector<Move>& moves) {
    // Score moves
    for (auto& move : moves) {
        move.score = getMoveScore(board, move);
    }
    
    // Sort by score
    std::sort(moves.begin(), moves.end(),
             [](const Move& a, const Move& b) { return a.score > b.score; });
             
    return moves;
}

float ChessEngine::alphaBeta(ChessBoard& board, int depth, float alpha, float beta, bool maximizing) {
    if (depth == 0) {
        return evaluatePosition(board);
    }
    
    std::vector<Move> moves = getPossibleMoves(board, maximizing ? WHITE : BLACK);
    moves = orderMoves(board, moves);  // Order moves before searching
    
    if (maximizing) {
        float maxEval = -std::numeric_limits<float>::infinity();
        for (const auto& move : moves) {
            // Make move
            ChessBoard newBoard = board;  // Create copy
            // TODO: Apply move to newBoard
            
            float eval = alphaBeta(newBoard, depth - 1, alpha, beta, false);
            maxEval = std::max(maxEval, eval);
            alpha = std::max(alpha, eval);
            if (beta <= alpha)
                break;
        }
        return maxEval;
    } else {
        float minEval = std::numeric_limits<float>::infinity();
        for (const auto& move : moves) {
            // Make move
            ChessBoard newBoard = board;  // Create copy
            // TODO: Apply move to newBoard
            
            float eval = alphaBeta(newBoard, depth - 1, alpha, beta, true);
            minEval = std::min(minEval, eval);
            beta = std::min(beta, eval);
            if (beta <= alpha)
                break;
        }
        return minEval;
    }
}

torch::Tensor ChessEngine::boardToTensor(const ChessBoard& board) {
    // Create 8x8x12 tensor (12 channels for different piece types)
    auto tensor = torch::zeros({12, 8, 8});
    
    // Fill tensor based on piece positions
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            ChessPiece* piece = board.getPieceAt(i, j);
            if (piece) {
                int channel = piece->getType() + (piece->getColor() == WHITE ? 0 : 6);
                tensor[channel][i][j] = 1;
            }
        }
    }
    
    return tensor;
}

float ChessEngine::evaluateKingSafety(const ChessBoard& board, PieceColor color) {
    float score = 0.0f;
    
    // Find king position
    int kingX = -1, kingY = -1;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            ChessPiece* piece = board.getPieceAt(i, j);
            if (piece && piece->getType() == KING && piece->getColor() == color) {
                kingX = i;
                kingY = j;
                break;
            }
        }
    }
    
    if (kingX == -1) return 0.0f;  // King not found
    
    // Pawn shield
    for (int dx = -1; dx <= 1; dx++) {
        int x = kingX + dx;
        int y = color == WHITE ? kingY - 1 : kingY + 1;
        
        if (x >= 0 && x < 8 && y >= 0 && y < 8) {
            ChessPiece* piece = board.getPieceAt(x, y);
            if (piece && piece->getType() == PAWN && piece->getColor() == color) {
                score += 0.5f;  // Bonus for each pawn protecting the king
            }
        }
    }
    
    // Piece proximity
    int protectingPieces = 0;
    for (int dx = -2; dx <= 2; dx++) {
        for (int dy = -2; dy <= 2; dy++) {
            int x = kingX + dx;
            int y = kingY + dy;
            
            if (x >= 0 && x < 8 && y >= 0 && y < 8) {
                ChessPiece* piece = board.getPieceAt(x, y);
                if (piece && piece->getColor() == color && piece->getType() != KING) {
                    protectingPieces++;
                }
            }
        }
    }
    score += protectingPieces * 0.2f;
    
    // Castling bonus
    if (board.hasKingMoved(color)) {
        score -= 1.0f;  // Penalty for losing castling rights
    }
    
    return score;
}

float ChessEngine::evaluatePawnStructure(const ChessBoard& board, PieceColor color) {
    float score = 0.0f;
    
    // Track pawns in each file
    int pawnsInFile[8] = {0};
    
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            ChessPiece* piece = board.getPieceAt(i, j);
            if (piece && piece->getType() == PAWN && piece->getColor() == color) {
                pawnsInFile[j]++;
                
                // Doubled pawns penalty
                if (pawnsInFile[j] > 1) {
                    score -= 0.5f;
                }
                
                // Isolated pawns penalty
                bool isolated = true;
                if (j > 0 && pawnsInFile[j-1] > 0) isolated = false;
                if (j < 7 && pawnsInFile[j+1] > 0) isolated = false;
                if (isolated) score -= 0.3f;
                
                // Connected pawns bonus
                if (j > 0) {
                    ChessPiece* neighbor = board.getPieceAt(i, j-1);
                    if (neighbor && neighbor->getType() == PAWN && neighbor->getColor() == color) {
                        score += 0.2f;
                    }
                }
                
                // Passed pawn bonus
                bool passed = true;
                int direction = (color == WHITE) ? -1 : 1;
                for (int row = i + direction; row >= 0 && row < 8; row += direction) {
                    for (int col = j-1; col <= j+1; col++) {
                        if (col >= 0 && col < 8) {
                            ChessPiece* blocker = board.getPieceAt(row, col);
                            if (blocker && blocker->getType() == PAWN && blocker->getColor() != color) {
                                passed = false;
                                break;
                            }
                        }
                    }
                }
                if (passed) score += 0.8f;
            }
        }
    }
    
    return score;
}

float ChessEngine::evaluatePieceActivity(const ChessBoard& board, PieceColor color) {
    float score = 0.0f;
    
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            ChessPiece* piece = board.getPieceAt(i, j);
            if (piece && piece->getColor() == color) {
                // Mobility
                auto moves = piece->getValidMoves(board);
                score += moves.size() * 0.1f;
                
                // Control of center squares
                for (const auto& move : moves) {
                    if (move.first >= 2 && move.first <= 5 && move.second >= 2 && move.second <= 5) {
                        score += 0.05f;
                    }
                }
                
                // Piece-specific bonuses
                switch (piece->getType()) {
                    case KNIGHT:
                        // Knights on outposts
                        if (i >= 3 && i <= 4 && !canBeAttackedByPawn(board, i, j, color)) {
                            score += 0.3f;
                        }
                        break;
                        
                    case BISHOP:
                        // Bishop pair bonus
                        if (hasBishopPair(board, color)) {
                            score += 0.5f;
                        }
                        break;
                        
                    case ROOK:
                        // Rooks on open files
                        if (isOpenFile(board, j)) {
                            score += 0.4f;
                        }
                        // Rooks on seventh rank
                        if ((color == WHITE && i == 6) || (color == BLACK && i == 1)) {
                            score += 0.3f;
                        }
                        break;
                }
            }
        }
    }
    
    return score;
}

// Helper functions for piece activity evaluation
bool ChessEngine::canBeAttackedByPawn(const ChessBoard& board, int x, int y, PieceColor color) {
    int pawnDirection = (color == WHITE) ? 1 : -1;
    for (int dx = -1; dx <= 1; dx += 2) {
        int px = x + dx;
        int py = y + pawnDirection;
        if (px >= 0 && px < 8 && py >= 0 && py < 8) {
            ChessPiece* piece = board.getPieceAt(px, py);
            if (piece && piece->getType() == PAWN && piece->getColor() != color) {
                return true;
            }
        }
    }
    return false;
}

bool ChessEngine::hasBishopPair(const ChessBoard& board, PieceColor color) {
    int bishops = 0;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            ChessPiece* piece = board.getPieceAt(i, j);
            if (piece && piece->getType() == BISHOP && piece->getColor() == color) {
                bishops++;
            }
        }
    }
    return bishops >= 2;
}

bool ChessEngine::isOpenFile(const ChessBoard& board, int file) {
    for (int i = 0; i < 8; i++) {
        ChessPiece* piece = board.getPieceAt(i, file);
        if (piece && piece->getType() == PAWN) {
            return false;
        }
    }
    return true;
}

float ChessEngine::calculateGamePhase(const ChessBoard& board) {
    int piecesLeft = 0;
    int totalPieces = 0;
    
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            ChessPiece* piece = board.getPieceAt(i, j);
            if (piece && piece->getType() != PAWN && piece->getType() != KING) {
                piecesLeft++;
                totalPieces++;
            }
        }
    }
    
    // Phase 1.0 = opening/middlegame, 0.0 = endgame
    return std::min(1.0f, piecesLeft / static_cast<float>(totalPieces));
} 