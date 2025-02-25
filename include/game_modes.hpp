#pragma once
#include <SFML/Graphics.hpp>
#include "board.hpp"
#include "ai_engine.hpp"
#include "online_manager.hpp"

enum class GameMode {
    LOCAL_AI,
    ONLINE_MULTIPLAYER,
    PRACTICE,
    TOURNAMENT
};

class GameModeManager {
public:
    GameModeManager(Board& board, AIEngine& ai, OnlineManager& online);
    
    void setMode(GameMode mode);
    void update();
    void handleMove(const Move& move);
    void toggleAnalysis();
    void toggleSpectator();
    
    // Mode-specific features
    void startTournamentMatch();
    void endTournamentMatch();
    void updateElo(bool won);
    void showMoveAnalysis();
    void saveMoveHistory();
    
private:
    Board& board;
    AIEngine& ai;
    OnlineManager& online;
    GameMode currentMode;
    bool analysisEnabled;
    bool isSpectating;
    int currentElo;
    std::vector<std::string> moveHistory;
}; 