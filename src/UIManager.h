#ifndef UIMANAGER_H
#define UIMANAGER_H

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <string>
#include <vector>
#include "GameEngine.h"

enum class UIState {
    WELCOME,
    GAMEPLAY,
    END_GAME
};

struct Button {
    int x, y, width, height;
    std::string label;
    bool hovered;
};

struct CardAnimation {
    Card card;
    int startX, startY;
    int endX, endY;
    int currentX, currentY;
    float progress; // 0.0 to 1.0
    bool active;
};

class UIManager {
public:
    UIManager();
    ~UIManager();
    
    bool initialize(int width, int height);
    void cleanup();
    
    void setGameEngine(GameEngine* engine) { m_gameEngine = engine; }
    
    void handleEvents();
    void update(float deltaTime);
    void render();
    
    bool isRunning() const { return m_running; }
    UIState getState() const { return m_state; }
    
private:
    // X11 resources
    Display* m_display;
    Window m_window;
    GC m_gc;
    int m_screen;
    unsigned long m_blackPixel;
    unsigned long m_whitePixel;
    unsigned long m_greenPixel;
    unsigned long m_redPixel;
    unsigned long m_bluePixel;
    
    // Window properties
    int m_width;
    int m_height;
    bool m_running;
    
    // UI state
    UIState m_state;
    GameEngine* m_gameEngine;
    
    // Buttons
    std::vector<Button> m_buttons;
    
    // Animations
    std::vector<CardAnimation> m_animations;
    
    // Game log
    std::vector<std::string> m_gameLog;
    int m_maxLogLines;
    
    // Mouse state
    int m_mouseX;
    int m_mouseY;
    
    // Rendering methods
    void renderWelcomeScreen();
    void renderGameplayScreen();
    void renderEndGameScreen();
    
    void drawButton(const Button& button);
    void drawCard(int x, int y, const Card& card, bool faceDown = false);
    void drawCardBack(int x, int y);
    void drawText(int x, int y, const std::string& text, bool large = false);
    void drawCenteredText(int y, const std::string& text, bool large = false);
    
    // UI helpers
    void createButton(int x, int y, int width, int height, const std::string& label);
    void clearButtons();
    bool isPointInButton(int x, int y, const Button& button) const;
    void updateButtonHover(int x, int y);
    void handleButtonClick(int x, int y);
    
    // Animation
    void addCardAnimation(const Card& card, int startX, int startY, int endX, int endY);
    void updateAnimations(float deltaTime);
    
    // Game event handling
    void onGameEvent(const GameEvent& event);
    void addLogMessage(const std::string& message);
    
    // Color allocation
    unsigned long allocateColor(int r, int g, int b);
};

#endif // UIMANAGER_H