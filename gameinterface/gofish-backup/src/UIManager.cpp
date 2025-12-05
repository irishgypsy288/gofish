#include "UIManager.h"
#include <cstring>
#include <cmath>
#include <iostream>
#include <algorithm>

// Constants
const int CARD_WIDTH = 80;
const int CARD_HEIGHT = 120;
const int CARD_SPACING = 10;
const int BUTTON_WIDTH = 200;
const int BUTTON_HEIGHT = 50;

UIManager::UIManager() 
    : m_display(nullptr), m_window(0), m_gc(0), m_screen(0),
      m_width(1024), m_height(768), m_running(false),
      m_state(UIState::WELCOME), m_gameEngine(nullptr),
      m_maxLogLines(10), m_mouseX(0), m_mouseY(0) {
}

UIManager::~UIManager() {
    cleanup();
}

bool UIManager::initialize(int width, int height) {
    m_width = width;
    m_height = height;
    
    // Open connection to X server
    m_display = XOpenDisplay(nullptr);
    if (!m_display) {
        std::cerr << "Cannot open X display" << std::endl;
        return false;
    }
    
    m_screen = DefaultScreen(m_display);
    m_blackPixel = BlackPixel(m_display, m_screen);
    m_whitePixel = WhitePixel(m_display, m_screen);
    
    // Allocate colors
    m_greenPixel = allocateColor(11, 93, 30);  // Dark green for table
    m_redPixel = allocateColor(220, 20, 60);   // Red for hearts/diamonds
    m_bluePixel = allocateColor(70, 130, 180); // Blue for card backs
    
    // Create window
    m_window = XCreateSimpleWindow(m_display, RootWindow(m_display, m_screen),
                                   100, 100, m_width, m_height, 1,
                                   m_blackPixel, m_greenPixel);
    
    // Set window properties
    XStoreName(m_display, m_window, "Go Fish - AI vs AI");
    
    // Select input events
    XSelectInput(m_display, m_window, ExposureMask | KeyPressMask | 
                 ButtonPressMask | PointerMotionMask | StructureNotifyMask);
    
    // Create graphics context
    m_gc = XCreateGC(m_display, m_window, 0, nullptr);
    XSetForeground(m_display, m_gc, m_whitePixel);
    XSetBackground(m_display, m_gc, m_greenPixel);
    
    // Map window
    XMapWindow(m_display, m_window);
    XFlush(m_display);
    
    m_running = true;
    return true;
}

void UIManager::cleanup() {
    if (m_display) {
        if (m_gc) {
            XFreeGC(m_display, m_gc);
        }
        if (m_window) {
            XDestroyWindow(m_display, m_window);
        }
        XCloseDisplay(m_display);
        m_display = nullptr;
    }
}

unsigned long UIManager::allocateColor(int r, int g, int b) {
    XColor color;
    Colormap colormap = DefaultColormap(m_display, m_screen);
    
    color.red = r * 257;
    color.green = g * 257;
    color.blue = b * 257;
    color.flags = DoRed | DoGreen | DoBlue;
    
    if (XAllocColor(m_display, colormap, &color)) {
        return color.pixel;
    }
    return m_whitePixel;
}

void UIManager::handleEvents() {
    XEvent event;
    
    while (XPending(m_display) > 0) {
        XNextEvent(m_display, &event);
        
        switch (event.type) {
            case Expose:
                if (event.xexpose.count == 0) {
                    render();
                }
                break;
                
            case KeyPress: {
                KeySym key = XLookupKeysym(&event.xkey, 0);
                if (key == XK_Escape) {
                    m_running = false;
                } else if (key == XK_Return || key == XK_space) {
                    if (m_state == UIState::WELCOME || m_state == UIState::END_GAME) {
                        // Start new game
                        if (m_gameEngine) {
                            m_gameEngine->startNewGame();
                            m_state = UIState::GAMEPLAY;
                            m_gameLog.clear();
                        }
                    }
                }
                break;
            }
                
            case ButtonPress:
                if (event.xbutton.button == Button1) {
                    handleButtonClick(event.xbutton.x, event.xbutton.y);
                }
                break;
                
            case MotionNotify:
                m_mouseX = event.xmotion.x;
                m_mouseY = event.xmotion.y;
                updateButtonHover(m_mouseX, m_mouseY);
                break;
                
            case ConfigureNotify:
                m_width = event.xconfigure.width;
                m_height = event.xconfigure.height;
                break;
        }
    }
}

void UIManager::update(float deltaTime) {
    updateAnimations(deltaTime);
    
    // Auto-step game if in gameplay state and no animations
    if (m_state == UIState::GAMEPLAY && m_animations.empty() && m_gameEngine) {
        if (m_gameEngine->getGameState() == GameState::PLAYING) {
            if (!m_gameEngine->stepGame()) {
                // Game ended
                m_state = UIState::END_GAME;
            }
        }
    }
}

void UIManager::render() {
    // Clear window
    XSetForeground(m_display, m_gc, m_greenPixel);
    XFillRectangle(m_display, m_window, m_gc, 0, 0, m_width, m_height);
    
    switch (m_state) {
        case UIState::WELCOME:
            renderWelcomeScreen();
            break;
        case UIState::GAMEPLAY:
            renderGameplayScreen();
            break;
        case UIState::END_GAME:
            renderEndGameScreen();
            break;
    }
    
    XFlush(m_display);
}

void UIManager::renderWelcomeScreen() {
    // Draw title
    XSetForeground(m_display, m_gc, m_whitePixel);
    drawCenteredText(m_height / 3, "GO FISH GAME", true);
    drawCenteredText(m_height / 3 + 40, "AI vs AI", false);
    
    // Create buttons
    clearButtons();
    createButton(m_width / 2 - BUTTON_WIDTH / 2, m_height / 2, 
                 BUTTON_WIDTH, BUTTON_HEIGHT, "NEW GAME");
    createButton(m_width / 2 - BUTTON_WIDTH / 2, m_height / 2 + 80, 
                 BUTTON_WIDTH, BUTTON_HEIGHT, "EXIT");
    
    // Draw buttons
    for (const auto& button : m_buttons) {
        drawButton(button);
    }
}

void UIManager::renderGameplayScreen() {
    if (!m_gameEngine) return;
    
    XSetForeground(m_display, m_gc, m_whitePixel);
    
    // Draw AI2 (top)
    drawText(20, 30, "AI2 (Opponent)", false);
    drawText(m_width - 150, 30, "Books: " + std::to_string(m_gameEngine->getBooks2()), false);
    
    const auto& hand2 = m_gameEngine->getHand2();
    int startX = (m_width - (hand2.size() * (CARD_WIDTH + CARD_SPACING))) / 2;
    for (size_t i = 0; i < hand2.size(); ++i) {
        drawCardBack(startX + i * (CARD_WIDTH + CARD_SPACING), 60);
    }
    
    // Draw game log
    int logY = m_height / 2 - 100;
    drawText(m_width / 2 - 200, logY, "Game Log:", false);
    XDrawRectangle(m_display, m_window, m_gc, m_width / 2 - 210, logY + 10, 420, 180);
    
    int logStartIdx = std::max(0, (int)m_gameLog.size() - m_maxLogLines);
    for (size_t i = logStartIdx; i < m_gameLog.size(); ++i) {
        drawText(m_width / 2 - 200, logY + 30 + (i - logStartIdx) * 18, 
                m_gameLog[i], false);
    }
    
    // Draw draw pile
    int pileX = m_width / 2 - CARD_WIDTH / 2;
    int pileY = m_height / 2 + 100;
    drawText(pileX - 50, pileY - 10, "Draw Pile: " + 
             std::to_string(m_gameEngine->getDrawPile().size()), false);
    if (!m_gameEngine->getDrawPile().empty()) {
        drawCardBack(pileX, pileY);
    }
    
    // Draw AI1 (bottom)
    int ai1Y = m_height - 180;
    drawText(20, ai1Y, "AI1 (Current Player)", false);
    drawText(m_width - 150, ai1Y, "Books: " + std::to_string(m_gameEngine->getBooks1()), false);
    
    const auto& hand1 = m_gameEngine->getHand1();
    startX = (m_width - (hand1.size() * (CARD_WIDTH + CARD_SPACING))) / 2;
    for (size_t i = 0; i < hand1.size(); ++i) {
        drawCard(startX + i * (CARD_WIDTH + CARD_SPACING), ai1Y + 30, hand1[i], false);
    }
    
    // Draw animations
    for (const auto& anim : m_animations) {
        if (anim.active) {
            drawCard(anim.currentX, anim.currentY, anim.card, false);
        }
    }
}

void UIManager::renderEndGameScreen() {
    if (!m_gameEngine) return;
    
    XSetForeground(m_display, m_gc, m_whitePixel);
    
    // Draw game over message
    drawCenteredText(m_height / 3, "GAME OVER!", true);
    
    std::string winner = m_gameEngine->getWinner();
    std::string winnerText = "Winner: " + winner;
    drawCenteredText(m_height / 3 + 60, winnerText, false);
    
    int winnerBooks = (winner == "AI1") ? m_gameEngine->getBooks1() : 
                      (winner == "AI2") ? m_gameEngine->getBooks2() : 
                      m_gameEngine->getBooks1();
    std::string booksText = "Books: " + std::to_string(winnerBooks);
    drawCenteredText(m_height / 3 + 90, booksText, false);
    
    // Create buttons
    clearButtons();
    createButton(m_width / 2 - BUTTON_WIDTH / 2, m_height / 2 + 50, 
                 BUTTON_WIDTH, BUTTON_HEIGHT, "NEW GAME");
    createButton(m_width / 2 - BUTTON_WIDTH / 2, m_height / 2 + 130, 
                 BUTTON_WIDTH, BUTTON_HEIGHT, "EXIT");
    
    // Draw buttons
    for (const auto& button : m_buttons) {
        drawButton(button);
    }
}

void UIManager::drawButton(const Button& button) {
    // Draw button background
    if (button.hovered) {
        unsigned long hoverColor = allocateColor(100, 150, 100);
        XSetForeground(m_display, m_gc, hoverColor);
    } else {
        unsigned long buttonColor = allocateColor(50, 100, 50);
        XSetForeground(m_display, m_gc, buttonColor);
    }
    XFillRectangle(m_display, m_window, m_gc, button.x, button.y, 
                   button.width, button.height);
    
    // Draw button border
    XSetForeground(m_display, m_gc, m_whitePixel);
    XDrawRectangle(m_display, m_window, m_gc, button.x, button.y, 
                   button.width, button.height);
    
    // Draw button text (centered)
    int textX = button.x + button.width / 2 - (button.label.length() * 6) / 2;
    int textY = button.y + button.height / 2 + 5;
    drawText(textX, textY, button.label, false);
}

void UIManager::drawCard(int x, int y, const Card& card, bool faceDown) {
    if (faceDown) {
        drawCardBack(x, y);
        return;
    }
    
    // Draw card background (white)
    XSetForeground(m_display, m_gc, m_whitePixel);
    XFillRectangle(m_display, m_window, m_gc, x, y, CARD_WIDTH, CARD_HEIGHT);
    
    // Draw card border
    XSetForeground(m_display, m_gc, m_blackPixel);
    XDrawRectangle(m_display, m_window, m_gc, x, y, CARD_WIDTH, CARD_HEIGHT);
    
    // Set color based on suit (red for hearts/diamonds, black for clubs/spades)
    if (card.suit == 0 || card.suit == 1) {
        XSetForeground(m_display, m_gc, m_redPixel);
    } else {
        XSetForeground(m_display, m_gc, m_blackPixel);
    }
    
    // Draw rank in corners
    std::string rankStr = GameEngine::rankToStr(card.rank);
    if (rankStr.length() > 2) {
        rankStr = rankStr.substr(0, 1); // Use first letter for face cards
    }
    drawText(x + 5, y + 15, rankStr, false);
    drawText(x + CARD_WIDTH - 15, y + CARD_HEIGHT - 5, rankStr, false);
    
    // Draw suit symbol in center
    std::string suitSymbol;
    switch (card.suit) {
        case 0: suitSymbol = "♥"; break;
        case 1: suitSymbol = "♦"; break;
        case 2: suitSymbol = "♣"; break;
        case 3: suitSymbol = "♠"; break;
    }
    drawText(x + CARD_WIDTH / 2 - 5, y + CARD_HEIGHT / 2 + 5, suitSymbol, true);
}

void UIManager::drawCardBack(int x, int y) {
    // Draw card background (blue)
    XSetForeground(m_display, m_gc, m_bluePixel);
    XFillRectangle(m_display, m_window, m_gc, x, y, CARD_WIDTH, CARD_HEIGHT);
    
    // Draw card border
    XSetForeground(m_display, m_gc, m_whitePixel);
    XDrawRectangle(m_display, m_window, m_gc, x, y, CARD_WIDTH, CARD_HEIGHT);
    
    // Draw pattern
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 4; ++j) {
            XDrawRectangle(m_display, m_window, m_gc, 
                          x + 10 + i * 20, y + 10 + j * 25, 15, 20);
        }
    }
}

void UIManager::drawText(int x, int y, const std::string& text, bool large) {
    XDrawString(m_display, m_window, m_gc, x, y, text.c_str(), text.length());
}

void UIManager::drawCenteredText(int y, const std::string& text, bool large) {
    int textWidth = text.length() * (large ? 10 : 8);
    int x = (m_width - textWidth) / 2;
    drawText(x, y, text, large);
}

void UIManager::createButton(int x, int y, int width, int height, const std::string& label) {
    Button button;
    button.x = x;
    button.y = y;
    button.width = width;
    button.height = height;
    button.label = label;
    button.hovered = false;
    m_buttons.push_back(button);
}

void UIManager::clearButtons() {
    m_buttons.clear();
}

bool UIManager::isPointInButton(int x, int y, const Button& button) const {
    return x >= button.x && x <= button.x + button.width &&
           y >= button.y && y <= button.y + button.height;
}

void UIManager::updateButtonHover(int x, int y) {
    bool needsRedraw = false;
    for (auto& button : m_buttons) {
        bool wasHovered = button.hovered;
        button.hovered = isPointInButton(x, y, button);
        if (wasHovered != button.hovered) {
            needsRedraw = true;
        }
    }
    if (needsRedraw) {
        render();
    }
}

void UIManager::handleButtonClick(int x, int y) {
    for (const auto& button : m_buttons) {
        if (isPointInButton(x, y, button)) {
            if (button.label == "NEW GAME") {
                if (m_gameEngine) {
                    m_gameEngine->startNewGame();
                    m_state = UIState::GAMEPLAY;
                    m_gameLog.clear();
                    render();
                }
            } else if (button.label == "EXIT") {
                m_running = false;
            }
        }
    }
}

void UIManager::addCardAnimation(const Card& card, int startX, int startY, int endX, int endY) {
    CardAnimation anim;
    anim.card = card;
    anim.startX = startX;
    anim.startY = startY;
    anim.endX = endX;
    anim.endY = endY;
    anim.currentX = startX;
    anim.currentY = startY;
    anim.progress = 0.0f;
    anim.active = true;
    m_animations.push_back(anim);
}

void UIManager::updateAnimations(float deltaTime) {
    bool needsRedraw = false;
    
    for (auto& anim : m_animations) {
        if (anim.active) {
            anim.progress += deltaTime * 2.0f; // 0.5 second animation
            
            if (anim.progress >= 1.0f) {
                anim.progress = 1.0f;
                anim.active = false;
            }
            
            // Ease-in-out interpolation
            float t = anim.progress;
            float eased = t < 0.5f ? 2.0f * t * t : -1.0f + (4.0f - 2.0f * t) * t;
            
            anim.currentX = anim.startX + (anim.endX - anim.startX) * eased;
            anim.currentY = anim.startY + (anim.endY - anim.startY) * eased;
            
            needsRedraw = true;
        }
    }
    
    // Remove completed animations
    m_animations.erase(
        std::remove_if(m_animations.begin(), m_animations.end(),
                      [](const CardAnimation& a) { return !a.active; }),
        m_animations.end()
    );
    
    if (needsRedraw) {
        render();
    }
}

void UIManager::onGameEvent(const GameEvent& event) {
    addLogMessage(event.message);
    
    // Trigger animations based on event type
    // (This would be expanded with actual card positions)
    
    render();
}

void UIManager::addLogMessage(const std::string& message) {
    m_gameLog.push_back(message);
    if (m_gameLog.size() > 50) {
        m_gameLog.erase(m_gameLog.begin());
    }
}