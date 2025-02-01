class BeginnerEngine : public ChessEngine {
public:
    BeginnerEngine() : ChessEngine("models/beginner.pt", 2) {}
    Move getBestMove(const ChessBoard& board) override;
};

class IntermediateEngine : public ChessEngine {
public:
    IntermediateEngine() : ChessEngine("models/intermediate.pt", 4) {}
    Move getBestMove(const ChessBoard& board) override;
};

class ExpertEngine : public ChessEngine {
public:
    ExpertEngine() : ChessEngine("models/expert.pt", 6) {}
    Move getBestMove(const ChessBoard& board) override;
}; 