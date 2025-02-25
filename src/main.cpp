#include <SFML/Graphics.hpp>
#include "ChessGame.h"
#include "ChessPiece.h"
#include <iostream>
#include <optional>
#include <variant>
#include "../include/game.hpp"
#include "../include/online_manager.hpp"

enum class GameState {
    MENU,
    MULTIPLAYER,
    AI_BEGINNER,
    AI_INTERMEDIATE,
    AI_EXPERT
};

class Game {
private:
    sf::Font font;  // Font needs to be initialized first
    sf::RenderWindow window;
    GameState currentState;
    ChessGame* game;  // Add ChessGame pointer
    
    // Menu buttons
    sf::RectangleShape multiplayerButton;
    sf::RectangleShape beginnerButton;
    sf::RectangleShape intermediateButton;
    sf::RectangleShape expertButton;
    
    sf::Text multiplayerText;
    sf::Text beginnerText;
    sf::Text intermediateText;
    sf::Text expertText;
    sf::Text titleText;

public:
    Game() : 
        font(),
        window(sf::VideoMode({800, 600}), "Chess Game"),
        currentState(GameState::MENU),
        game(nullptr),  // Initialize game pointer
        multiplayerText(font, ""),
        beginnerText(font, ""),
        intermediateText(font, ""),
        expertText(font, ""),
        titleText(font, "")
    {
        // Load font first
        if (!font.openFromFile("arial.ttf")) {
            std::cerr << "Error loading font!" << std::endl;
            return;
        }

        // Setup title
        titleText.setString("Chess Game");
        titleText.setCharacterSize(40);
        titleText.setFillColor(sf::Color::White);
        titleText.setPosition(sf::Vector2f(300.f, 50.f));

        // Setup buttons
        setupButton(multiplayerButton, multiplayerText, "Multiplayer", 300.f, 150.f);
        setupButton(beginnerButton, beginnerText, "AI Beginner", 300.f, 250.f);
        setupButton(intermediateButton, intermediateText, "AI Intermediate", 300.f, 350.f);
        setupButton(expertButton, expertText, "AI Expert", 300.f, 450.f);
    }

    ~Game() {
        if (game) delete game;
    }

    void setupButton(sf::RectangleShape& button, sf::Text& text, const std::string& str, float x, float y) {
        button.setSize(sf::Vector2f(200.f, 50.f));
        button.setPosition(sf::Vector2f(x, y));
        button.setFillColor(sf::Color(100, 100, 100));

        text.setString(str);
        text.setCharacterSize(20);
        text.setFillColor(sf::Color::White);
        text.setPosition(sf::Vector2f(x + 20.f, y + 10.f));
    }

    void run() {
        while (window.isOpen()) {
            handleEvents();
            update();
            render();
        }
    }

private:
    void handleEvents() {
        if (currentState == GameState::MENU) {
            handleMenuEvents();
        } else {
            handleGameEvents();
        }
    }

    void handleMenuEvents() {
        // According to docs, pollEvent returns std::optional<Event>
        while (const std::optional<sf::Event> event = window.pollEvent()) {
            // Check if event is a window close
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
            // Check if event is a mouse button press
            else if (event->is<sf::Event::MouseButtonPressed>()) {
                auto mousePos = sf::Mouse::getPosition(window);
                handleMouseClick(mousePos.x, mousePos.y);
            }
        }
    }

    void handleGameEvents() {
        sf::Vector2i position = sf::Mouse::getPosition(window);
        int x = position.x / size;
        int y = position.y / size;

        while (const std::optional<sf::Event> maybeEvent = window.pollEvent()) {
            const auto& e = *maybeEvent;
            
            switch (e.type) {
                case sf::Event::Closed:
                    window.close();
                    break;
                
                case sf::Event::MouseButtonPressed:
                    if (e.mouseButton.button == sf::Mouse::Button::Left) {
                        for (int i = 0; i < 8; i++) {
                            for (int j = 0; j < 8; j++) {
                                if (game->board->getPieceAt(j, i) != nullptr && 
                                    x == j && y == i && 
                                    game->board->getColorAt(j, i) == game->currTurn && 
                                    !game->isPromoting()) {
                                    game->movingPiece = game->board->getPieceAt(x, y);
                                }
                            }
                        }
                        if (game->isPromoting()) {
                            game->promotePiece(position);
                        }
                    }
                    break;
                
                case sf::Event::KeyPressed:
                    if (e.key.code == sf::Keyboard::Key::R) {
                        game->board->initializeBoard(game->board->bottomPlayer);
                        game->currTurn = WHITE;
                    }
                    break;
            }
        }
    }

    void handleMouseClick(int x, int y) {
        if (currentState == GameState::MENU) {
            sf::Vector2f mousePos(static_cast<float>(x), static_cast<float>(y));
            
            if (multiplayerButton.getGlobalBounds().contains(mousePos)) {
                currentState = GameState::MULTIPLAYER;
            }
            else if (beginnerButton.getGlobalBounds().contains(mousePos)) {
                currentState = GameState::AI_BEGINNER;
            }
            else if (intermediateButton.getGlobalBounds().contains(mousePos)) {
                currentState = GameState::AI_INTERMEDIATE;
            }
            else if (expertButton.getGlobalBounds().contains(mousePos)) {
                currentState = GameState::AI_EXPERT;
            }
        }
    }

    void handlePieceMove(int x, int y) {
        int movingPieceX = game->movingPiece->getPiecePosition(game->board).first;
        int movingPieceY = game->movingPiece->getPiecePosition(game->board).second;
        
        if (game->currTurn == game->movingPiece->pieceColor && 
            game->movingPiece->isValidMove(game->board, x, y) && 
            !game->board->nextMoveIsChecked(game->movingPiece, x, y)) {
            
            game->unsetEnPassantPieces();
            game->currTurn = getOtherColor(game->currTurn);
            game->setSoundFromMove(game->moveSoundBuffer, game->moveSound, game->movingPiece, x, y);
            game->moveSound->play();
            game->movingPiece->movePiece(game->board, x, y);
        } else {
            game->movingPiece->pieceSprite.setPosition(sf::Vector2f(size * movingPieceX, size * movingPieceY));
        }
        game->movingPiece = nullptr;
    }

    void update() {
        if (currentState != GameState::MENU && game != nullptr) {
            if (game->isCheckmated(WHITE)) {
                std::cout << "BLACK WINS" << std::endl;
            }
            if (game->isCheckmated(BLACK)) {
                std::cout << "WHITE WINS" << std::endl;
            }
            if (game->isStalemated(WHITE) || game->isStalemated(BLACK)) {
                std::cout << "STALEMATE" << std::endl;
            }
        }
    }

    void render() {
        window.clear(sf::Color(50, 50, 50));

        if (currentState == GameState::MENU) {
            renderMenu();
        } else {
            renderGame();
        }

        window.display();
    }

    void renderMenu() {
        window.draw(titleText);
        window.draw(multiplayerButton);
        window.draw(multiplayerText);
        window.draw(beginnerButton);
        window.draw(beginnerText);
        window.draw(intermediateButton);
        window.draw(intermediateText);
        window.draw(expertButton);
        window.draw(expertText);
    }

    void renderGame() {
        if (!game) return;
        
        game->drawBoard();
        game->drawPossibleMoves();
        game->drawPieces();
        
        if (game->movingPiece != nullptr) {
            sf::Vector2i position = sf::Mouse::getPosition(window);
            game->movingPiece->pieceSprite.setPosition(sf::Vector2f(position.x - size / 2, position.y - size / 2));
            window.draw(game->movingPiece->pieceSprite);
        }
    }
};

int main() {
    Game game;
    OnlineManager onlineManager;
    
    if (game.initialize()) {
        game.run();
    }
    
    return 0;
}