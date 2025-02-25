#include "game_modes.hpp"
#include <fstream>
#include <json/json.h>

GameModeManager::GameModeManager(Board& board, AIEngine& ai, OnlineManager& online)
    : board(board), ai(ai), online(online), 
      currentMode(GameMode::LOCAL_AI),
      analysisEnabled(false),
      isSpectating(false),
      currentElo(1200)
{}

void GameModeManager::setMode(GameMode mode) {
    currentMode = mode;
    switch (mode) {
        case GameMode::TOURNAMENT:
            startTournamentMatch();
            break;
        case GameMode::ONLINE_MULTIPLAYER:
            online.connect("wss://your-api-endpoint");
            break;
        case GameMode::PRACTICE:
            analysisEnabled = true;
            ai.setDifficulty(AIEngine::Difficulty::INTERMEDIATE);
            break;
        default:
            break;
    }
}

void GameModeManager::update() {
    if (currentMode == GameMode::ONLINE_MULTIPLAYER && !isSpectating) {
        online.update();
    }
    
    if (analysisEnabled) {
        showMoveAnalysis();
    }
}

void GameModeManager::handleMove(const Move& move) {
    // Record move in history
    moveHistory.push_back(move.toAlgebraic());
    
    // Handle online moves
    if (currentMode == GameMode::ONLINE_MULTIPLAYER) {
        online.makeMove(move);
    }
    
    // Save move history
    saveMoveHistory();
}

void GameModeManager::toggleAnalysis() {
    analysisEnabled = !analysisEnabled;
    if (analysisEnabled) {
        showMoveAnalysis();
    }
}

void GameModeManager::toggleSpectator() {
    if (currentMode == GameMode::ONLINE_MULTIPLAYER) {
        isSpectating = !isSpectating;
    }
}

void GameModeManager::startTournamentMatch() {
    // Find opponent with similar ELO
    Json::Value request;
    request["elo"] = currentElo;
    online.findMatch(currentElo);
}

void GameModeManager::endTournamentMatch() {
    if (currentMode == GameMode::TOURNAMENT) {
        saveMoveHistory();
    }
}

void GameModeManager::updateElo(bool won) {
    int eloChange = won ? 15 : -15;
    currentElo += eloChange;
    
    // Update online ELO
    if (currentMode == GameMode::TOURNAMENT) {
        online.updateElo(currentElo);
    }
}

void GameModeManager::showMoveAnalysis() {
    if (!analysisEnabled) return;
    
    // Get current position evaluation
    float eval = ai.evaluatePosition(board);
    
    // Get best move suggestion
    Move bestMove = ai.getBestMove(board);
    
    // These values would be used by the UI to display analysis
}

void GameModeManager::saveMoveHistory() {
    std::ofstream file("game_history.pgn", std::ios::app);
    for (const auto& move : moveHistory) {
        file << move << " ";
    }
    file << std::endl;
} 