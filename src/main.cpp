#include <iostream>
#include <chrono>
#include <thread>
#include "GameEngine.h"
#include "UIManager.h"
#include "AudioManager.h"

int main() {
    std::cout << "Starting Go Fish Game..." << std::endl;
    
    // Create game engine
    GameEngine gameEngine;
    
    // Create UI manager
    UIManager uiManager;
    if (!uiManager.initialize(1024, 768)) {
        std::cerr << "Failed to initialize UI" << std::endl;
        return 1;
    }
    
    // Create audio manager
    AudioManager audioManager;
    audioManager.initialize();
    
    // Connect game engine to UI
    uiManager.setGameEngine(&gameEngine);
    
    // Set up game event callback
    gameEngine.setEventCallback([&uiManager, &audioManager](const GameEvent& event) {
        // Handle game events
        if (event.type == EventType::GAME_ENDED) {
            // Play victory sound
            audioManager.playSound("victory");
        } else if (event.type == EventType::CARDS_TRANSFERRED) {
            // Play card move sound
            audioManager.playSound("card_move");
        }
    });
    
    // Main game loop
    auto lastTime = std::chrono::high_resolution_clock::now();
    
    while (uiManager.isRunning()) {
        auto currentTime = std::chrono::high_resolution_clock::now();
        float deltaTime = std::chrono::duration<float>(currentTime - lastTime).count();
        lastTime = currentTime;
        
        // Handle events
        uiManager.handleEvents();
        
        // Update game state
        uiManager.update(deltaTime);
        
        // Render
        uiManager.render();
        
        // Cap frame rate to ~60 FPS
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
    
    std::cout << "Game closed. Goodbye!" << std::endl;
    return 0;
}