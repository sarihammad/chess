#pragma once
#include "../piece.hpp"

class King : public Piece {
public:
    King(Color color, const std::string& texturePath);
    bool isValidMove(const Board& board, const Move& move) const override;
    std::vector<Move> getPossibleMoves(const Board& board) const override;
    bool canCastle(const Board& board, bool kingSide) const;
    
private:
    bool hasMoved;
    friend class Board;  // Allow Board to update hasMoved
}; 