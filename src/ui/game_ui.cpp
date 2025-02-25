#include "ui/game_ui.hpp"
#include <sstream>
#include <iomanip>

GameUI::GameUI(sf::RenderWindow& window, Board& board, GameModeManager& gameModes)
    : window(window), board(board), gameModes(gameModes),
      isDragging(false), selectedPiece(nullptr)
{
    // Load font
    font.loadFromFile("resources/fonts/arial.ttf");
    
    // Setup UI elements
    analysisPanel.setSize(sf::Vector2f(200, window.getSize().y));
    analysisPanel.setPosition(window.getSize().x - 200, 0);
    analysisPanel.setFillColor(sf::Color(50, 50, 50));
    
    evaluationText.setFont(font);
    moveHistoryText.setFont(font);
    eloText.setFont(font);
    
    evaluationText.setCharacterSize(14);
    moveHistoryText.setCharacterSize(14);
    eloText.setCharacterSize(14);
    
    evaluationText.setPosition(window.getSize().x - 190, 10);
    moveHistoryText.setPosition(window.getSize().x - 190, 50);
    eloText.setPosition(window.getSize().x - 190, window.getSize().y - 30);
}

void GameUI::update() {
    updateAnalysis();
}

void GameUI::draw() {
    drawBoard();
    drawPieces();
    
    // Draw UI panels
    window.draw(analysisPanel);
    drawAnalysis();
    drawMoveHistory();
    drawEloRating();
    
    // Draw dragged piece
    if (isDragging && selectedPiece) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        selectedPiece->draw(window, mousePos.x - 32, mousePos.y - 32);  // Center piece on cursor
    }
}

void GameUI::handleEvent(const sf::Event& event) {
    switch (event.type) {
        case sf::Event::MouseButtonPressed:
            if (event.mouseButton.button == sf::Mouse::Left) {
                handleDragAndDrop(event);
            }
            else if (event.mouseButton.button == sf::Mouse::Right) {
                gameModes.toggleAnalysis();
            }
            break;
            
        case sf::Event::MouseButtonReleased:
            if (event.mouseButton.button == sf::Mouse::Left && isDragging) {
                sf::Vector2i pos = sf::Mouse::getPosition(window);
                int toX = pos.x / 64;  // Assuming 64px squares
                int toY = pos.y / 64;
                
                if (selectedPiece) {
                    Move move(dragStart.x, dragStart.y, toX, toY);
                    if (board.isValidMove(move)) {
                        board.makeMove(move);
                        gameModes.handleMove(move);
                    }
                }
                
                isDragging = false;
                selectedPiece = nullptr;
            }
            break;
            
        case sf::Event::KeyPressed:
            switch (event.key.code) {
                case sf::Keyboard::F:
                    board.flip();
                    break;
                case sf::Keyboard::R:
                    board.reset();
                    break;
                case sf::Keyboard::U:
                    board.undoMove();
                    break;
                case sf::Keyboard::S:
                    board.saveGame("saved_game.pgn");
                    break;
                case sf::Keyboard::L:
                    board.loadGame("saved_game.pgn");
                    break;
                case sf::Keyboard::Tab:
                    gameModes.toggleSpectator();
                    break;
                default:
                    break;
            }
            break;
            
        default:
            break;
    }
}

void GameUI::drawBoard() {
    // Draw chess board squares
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            sf::RectangleShape square(sf::Vector2f(64, 64));
            square.setPosition(i * 64, j * 64);
            square.setFillColor((i + j) % 2 == 0 ? sf::Color(238, 238, 210) : sf::Color(118, 150, 86));
            window.draw(square);
        }
    }
}

void GameUI::drawPieces() {
    board.draw(window);
}

void GameUI::drawAnalysis() {
    if (!gameModes.isAnalysisEnabled()) return;
    
    std::stringstream ss;
    ss << std::fixed << std::setprecision(2) << board.getEvaluation();
    evaluationText.setString("Evaluation: " + ss.str());
    window.draw(evaluationText);
}

void GameUI::drawMoveHistory() {
    const auto& history = board.getMoveHistory();
    std::string historyText;
    for (size_t i = 0; i < history.size(); i++) {
        if (i % 2 == 0) {
            historyText += std::to_string(i/2 + 1) + ". ";
        }
        historyText += history[i] + " ";
    }
    moveHistoryText.setString(historyText);
    window.draw(moveHistoryText);
}

void GameUI::drawEloRating() {
    eloText.setString("ELO: " + std::to_string(board.getCurrentElo()));
    window.draw(eloText);
}

void GameUI::handleDragAndDrop(const sf::Event& event) {
    sf::Vector2i pos(event.mouseButton.x, event.mouseButton.y);
    int x = pos.x / 64;
    int y = pos.y / 64;
    
    if (x >= 0 && x < 8 && y >= 0 && y < 8) {
        selectedPiece = board.getPieceAt(x, y);
        if (selectedPiece && selectedPiece->getColor() == board.getCurrentTurn()) {
            isDragging = true;
            dragStart = sf::Vector2i(x, y);
        }
    }
}

void GameUI::updateAnalysis() {
    if (gameModes.isAnalysisEnabled()) {
        // Analysis is updated automatically by GameModeManager
    }
} 