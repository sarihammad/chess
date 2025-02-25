#pragma once
#include <SFML/Graphics.hpp>
#include "../board.hpp"
#include "../game_modes.hpp"

class GameUI {
public:
    GameUI(sf::RenderWindow& window, Board& board, GameModeManager& gameModes);
    
    void update();
    void draw();
    void handleEvent(const sf::Event& event);
    
private:
    sf::RenderWindow& window;
    Board& board;
    GameModeManager& gameModes;
    
    // UI Elements
    sf::RectangleShape analysisPanel;
    sf::Text evaluationText;
    sf::Text moveHistoryText;
    sf::Text eloText;
    sf::Font font;
    
    // UI States
    bool isDragging;
    sf::Vector2i dragStart;
    Piece* selectedPiece;
    
    // UI Methods
    void drawBoard();
    void drawPieces();
    void drawAnalysis();
    void drawMoveHistory();
    void drawEloRating();
    void handleDragAndDrop(const sf::Event& event);
    void updateAnalysis();
}; 