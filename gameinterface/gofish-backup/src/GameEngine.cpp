#include "GameEngine.h"
#include <algorithm>
#include <random>
#include <iostream>

bool operator<(const Card& a, const Card& b) {
    if (a.rank != b.rank) return a.rank < b.rank;
    return a.suit < b.suit;
}

GameEngine::GameEngine() 
    : m_books1(0), m_books2(0), m_turn("AI1"), m_gameState(GameState::NOT_STARTED) {
    m_deniedRanks["AI1"] = {};
    m_deniedRanks["AI2"] = {};
}

GameEngine::~GameEngine() {
}

void GameEngine::startNewGame() {
    reset();
    initializeDeck();
    dealCards();
    
    // Check for initial books
    m_books1 += checkBooks(m_hand1);
    m_books2 += checkBooks(m_hand2);
    
    m_gameState = GameState::PLAYING;
    m_turn = "AI1";
    
    GameEvent event;
    event.type = EventType::GAME_STARTED;
    event.message = "New game started!";
    emitEvent(event);
}

void GameEngine::reset() {
    m_hand1.clear();
    m_hand2.clear();
    m_drawPile.clear();
    m_books1 = 0;
    m_books2 = 0;
    m_turn = "AI1";
    m_winner = "";
    m_gameState = GameState::NOT_STARTED;
    m_deniedRanks["AI1"].clear();
    m_deniedRanks["AI2"].clear();
    m_eventHistory.clear();
}

void GameEngine::initializeDeck() {
    std::vector<Card> deck;
    for (int suit = 0; suit < 4; ++suit) {
        for (int rank = 1; rank <= 13; ++rank) {
            deck.push_back({rank, suit});
        }
    }
    
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(deck.begin(), deck.end(), g);
    
    m_drawPile = deck;
}

void GameEngine::dealCards() {
    // Deal 7 cards to each player
    m_hand1.assign(m_drawPile.begin(), m_drawPile.begin() + 7);
    m_hand2.assign(m_drawPile.begin() + 7, m_drawPile.begin() + 14);
    m_drawPile.erase(m_drawPile.begin(), m_drawPile.begin() + 14);
    
    std::sort(m_hand1.begin(), m_hand1.end());
    std::sort(m_hand2.begin(), m_hand2.end());
}

int GameEngine::checkBooks(std::vector<Card>& hand) {
    int newBooks = 0;
    for (int r = 1; r <= 13; ++r) {
        if (countCards(hand, r) == 4) {
            // Remove the 4 cards of rank r
            hand.erase(std::remove_if(hand.begin(), hand.end(), 
                [r](const Card& c) { return c.rank == r; }), hand.end());
            ++newBooks;
            
            GameEvent event;
            event.type = EventType::BOOK_FORMED;
            event.rank = r;
            event.message = "Book of " + pluralRank(r) + " formed!";
            emitEvent(event);
        }
    }
    std::sort(hand.begin(), hand.end());
    return newBooks;
}

int GameEngine::countCards(const std::vector<Card>& hand, int rank) const {
    int count = 0;
    for (const auto& c : hand) {
        if (c.rank == rank) ++count;
    }
    return count;
}

int GameEngine::chooseRank(const std::vector<Card>& hand, const std::set<int>& denied) const {
    std::set<int> available;
    for (const auto& c : hand) {
        if (denied.find(c.rank) == denied.end()) {
            available.insert(c.rank);
        }
    }
    if (available.empty()) {
        return 0;
    }
    return *available.begin();
}

bool GameEngine::validateRequest(const std::vector<Card>& hand, int rank) const {
    for (const auto& c : hand) {
        if (c.rank == rank) return true;
    }
    return false;
}

void GameEngine::processRequest(const std::string& player, const std::string& opponent, int rank,
                                std::vector<Card>& pHand, std::vector<Card>& oHand, int& pBooks) {
    int num = countCards(oHand, rank);
    
    GameEvent event;
    event.type = EventType::CARDS_TRANSFERRED;
    event.player = player;
    event.opponent = opponent;
    event.rank = rank;
    event.count = num;
    event.message = opponent + " gives " + std::to_string(num) + " " + pluralRank(rank) + " to " + player;
    emitEvent(event);
    
    // Transfer cards
    std::vector<Card> transfer;
    for (auto it = oHand.begin(); it != oHand.end(); ) {
        if (it->rank == rank) {
            transfer.push_back(*it);
            it = oHand.erase(it);
        } else {
            ++it;
        }
    }
    
    for (const auto& c : transfer) {
        pHand.push_back(c);
    }
    
    std::sort(pHand.begin(), pHand.end());
    std::sort(oHand.begin(), oHand.end());
    
    pBooks += checkBooks(pHand);
    m_deniedRanks[player].clear();
}

bool GameEngine::noValidMoves() const {
    int r1 = chooseRank(m_hand1, m_deniedRanks.at("AI1"));
    int r2 = chooseRank(m_hand2, m_deniedRanks.at("AI2"));
    return r1 == 0 && r2 == 0;
}

bool GameEngine::isGameOver() const {
    return (m_drawPile.empty() && (m_hand1.empty() || m_hand2.empty())) ||
           (m_hand1.empty() && m_hand2.empty()) ||
           (m_drawPile.empty() && noValidMoves());
}

bool GameEngine::stepGame() {
    if (m_gameState != GameState::PLAYING) {
        return false;
    }
    
    // Check end conditions
    if (isGameOver()) {
        if (m_books1 > m_books2) {
            m_winner = "AI1";
        } else if (m_books2 > m_books1) {
            m_winner = "AI2";
        } else {
            m_winner = "Tie";
        }
        
        m_gameState = GameState::GAME_OVER;
        
        GameEvent event;
        event.type = EventType::GAME_ENDED;
        event.message = "Game Over! Winner: " + m_winner;
        emitEvent(event);
        
        return false;
    }
    
    std::string p = m_turn;
    std::string o = (p == "AI1" ? "AI2" : "AI1");
    std::vector<Card>& pHand = (p == "AI1" ? m_hand1 : m_hand2);
    std::vector<Card>& oHand = (p == "AI1" ? m_hand2 : m_hand1);
    int& pBooks = (p == "AI1" ? m_books1 : m_books2);
    
    GameEvent turnEvent;
    turnEvent.type = EventType::TURN_STARTED;
    turnEvent.player = p;
    turnEvent.message = p + "'s turn";
    emitEvent(turnEvent);
    
    if (pHand.empty()) {
        m_turn = o;
        return true;
    }
    
    std::set<int> denied = m_deniedRanks[p];
    int r = chooseRank(pHand, denied);
    
    if (r == 0 || !validateRequest(pHand, r)) {
        m_turn = o;
        return true;
    }
    
    GameEvent requestEvent;
    requestEvent.type = EventType::REQUEST_MADE;
    requestEvent.player = p;
    requestEvent.opponent = o;
    requestEvent.rank = r;
    requestEvent.message = p + " asks " + o + " for " + pluralRank(r);
    emitEvent(requestEvent);
    
    bool hasRank = countCards(oHand, r) > 0;
    
    if (hasRank) {
        processRequest(p, o, r, pHand, oHand, pBooks);
        m_turn = p; // Go again
    } else {
        GameEvent goFishEvent;
        goFishEvent.type = EventType::GO_FISH;
        goFishEvent.player = p;
        goFishEvent.opponent = o;
        goFishEvent.message = o + " says: Go Fish!";
        emitEvent(goFishEvent);
        
        m_deniedRanks[p].insert(r);
        
        if (!m_drawPile.empty()) {
            Card card = m_drawPile.front();
            m_drawPile.erase(m_drawPile.begin());
            pHand.push_back(card);
            std::sort(pHand.begin(), pHand.end());
            
            GameEvent drawEvent;
            drawEvent.type = EventType::CARD_DRAWN;
            drawEvent.player = p;
            drawEvent.drawnCard = card;
            drawEvent.message = p + " draws: " + cardToStr(card);
            emitEvent(drawEvent);
            
            pBooks += checkBooks(pHand);
            
            if (card.rank == r) {
                m_turn = p; // Go again
            } else {
                m_turn = o;
            }
            
            m_deniedRanks[p].clear();
        } else {
            m_turn = o;
        }
    }
    
    return true;
}

void GameEngine::emitEvent(const GameEvent& event) {
    m_eventHistory.push_back(event);
    if (m_eventCallback) {
        m_eventCallback(event);
    }
}

const GameEvent& GameEngine::getLastEvent() const {
    static GameEvent emptyEvent;
    if (m_eventHistory.empty()) {
        return emptyEvent;
    }
    return m_eventHistory.back();
}

std::string GameEngine::rankToStr(int rank) {
    if (rank == 1) return "Ace";
    if (rank == 11) return "Jack";
    if (rank == 12) return "Queen";
    if (rank == 13) return "King";
    return std::to_string(rank);
}

std::string GameEngine::pluralRank(int rank) {
    if (rank == 1) return "Aces";
    if (rank == 11) return "Jacks";
    if (rank == 12) return "Queens";
    if (rank == 13) return "Kings";
    return std::to_string(rank) + "s";
}

std::string GameEngine::suitToStr(int suit) {
    std::string suits[4] = {"Hearts", "Diamonds", "Clubs", "Spades"};
    return suits[suit];
}

std::string GameEngine::cardToStr(const Card& card) {
    return rankToStr(card.rank) + " of " + suitToStr(card.suit);
}