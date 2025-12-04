#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include <vector>
#include <string>
#include <map>
#include <set>
#include <functional>

struct Card {
    int rank; // 1-13 (1=A, 11=J, 12=Q, 13=K)
    int suit; // 0=Hearts, 1=Diamonds, 2=Clubs, 3=Spades
};

bool operator<(const Card& a, const Card& b);

enum class GameState {
    NOT_STARTED,
    PLAYING,
    GAME_OVER
};

enum class EventType {
    GAME_STARTED,
    TURN_STARTED,
    REQUEST_MADE,
    CARDS_TRANSFERRED,
    GO_FISH,
    CARD_DRAWN,
    BOOK_FORMED,
    TURN_ENDED,
    GAME_ENDED
};

struct GameEvent {
    EventType type;
    std::string player;
    std::string opponent;
    int rank;
    int count;
    Card drawnCard;
    std::string message;
};

class GameEngine {
public:
    GameEngine();
    ~GameEngine();
    
    // Game control
    void startNewGame();
    bool stepGame(); // Execute one game step, returns false if game over
    void reset();
    
    // State queries
    GameState getGameState() const { return m_gameState; }
    const std::vector<Card>& getHand1() const { return m_hand1; }
    const std::vector<Card>& getHand2() const { return m_hand2; }
    const std::vector<Card>& getDrawPile() const { return m_drawPile; }
    int getBooks1() const { return m_books1; }
    int getBooks2() const { return m_books2; }
    std::string getCurrentTurn() const { return m_turn; }
    std::string getWinner() const { return m_winner; }
    const std::vector<GameEvent>& getEventHistory() const { return m_eventHistory; }
    const GameEvent& getLastEvent() const;
    
    // Event callback system
    void setEventCallback(std::function<void(const GameEvent&)> callback) {
        m_eventCallback = callback;
    }
    
    // Utility functions
    static std::string rankToStr(int rank);
    static std::string pluralRank(int rank);
    static std::string suitToStr(int suit);
    static std::string cardToStr(const Card& card);
    
private:
    // Game state
    std::vector<Card> m_hand1;
    std::vector<Card> m_hand2;
    std::vector<Card> m_drawPile;
    int m_books1;
    int m_books2;
    std::string m_turn;
    std::string m_winner;
    GameState m_gameState;
    std::map<std::string, std::set<int>> m_deniedRanks;
    std::vector<GameEvent> m_eventHistory;
    std::function<void(const GameEvent&)> m_eventCallback;
    
    // Internal game logic
    void initializeDeck();
    void dealCards();
    int checkBooks(std::vector<Card>& hand);
    int countCards(const std::vector<Card>& hand, int rank) const;
    int chooseRank(const std::vector<Card>& hand, const std::set<int>& denied) const;
    bool validateRequest(const std::vector<Card>& hand, int rank) const;
    void processRequest(const std::string& player, const std::string& opponent, int rank,
                       std::vector<Card>& pHand, std::vector<Card>& oHand, int& pBooks);
    bool noValidMoves() const;
    bool isGameOver() const;
    void emitEvent(const GameEvent& event);
};

#endif // GAMEENGINE_H