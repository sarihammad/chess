#pragma once
#include <SFML/Graphics.hpp>
#include "board.hpp"
#include "ai_engine.hpp"
#include "online_manager.hpp"

class Game {
public:
    Game();
    bool initialize();
    void run();
    void handleEvents();
    void update();
    void render();
    
private:
    sf::RenderWindow window;
    Board board;
    AIEngine aiEngine;
    bool isOnlineMode;
    GameState state;
    
    void toggleOnlineMode();
    void handleMouseInput(const sf::Event& event);
    void handleKeyboardInput(const sf::Event& event);
}; 