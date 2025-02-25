// Modify constructor to initialize AI based on game state:
void Game::initializeAI() {
    switch (currentState) {
        case GameState::AI_BEGINNER:
            ai = std::make_unique<BeginnerEngine>();
            break;
        case GameState::AI_INTERMEDIATE:
            ai = std::make_unique<IntermediateEngine>();
            break;
        case GameState::AI_EXPERT:
            ai = std::make_unique<ExpertEngine>();
            break;
        default:
            ai = nullptr;
    }
}

// Add to handleGameEvents:
void Game::handleGameEvents() {
    // ... existing event handling ...
    
    // AI move if it's AI's turn
    if (ai && game && game->currTurn == BLACK) {
        Move aiMove = ai->getBestMove(*(game->board));
        
        // Apply AI's move
        ChessPiece* piece = game->board->getPieceAt(aiMove.fromX, aiMove.fromY);
        if (piece) {
            piece->movePiece(game->board, aiMove.toX, aiMove.toY);
            game->currTurn = WHITE;
            
            // Play move sound
            game->setSoundFromMove(game->moveSoundBuffer, game->moveSound, 
                                 piece, aiMove.toX, aiMove.toY);
            game->moveSound->play();
        }
    }
}

// Add to handleMouseClick:
void Game::handleMouseClick(int x, int y) {
    if (currentState == GameState::MENU) {
        // ... existing menu handling ...
        
        // Initialize AI when game mode is selected
        initializeAI();
    }
} 