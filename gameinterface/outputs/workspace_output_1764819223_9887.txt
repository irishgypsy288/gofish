#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <algorithm>
#include <random>

struct Card {
    int rank; // 1-13 (1=A, 11=J, 12=Q, 13=K)
    int suit; // 0=Hearts, 1=Diamonds, 2=Clubs, 3=Spades
};

bool operator<(const Card& a, const Card& b) {
    if (a.rank != b.rank) return a.rank < b.rank;
    return a.suit < b.suit;
}

struct State {
    std::vector<Card> hand1;
    std::vector<Card> hand2;
    std::vector<Card> drawPile;
    int books1 = 0;
    int books2 = 0;
    std::string turn;
    std::string winner;
    std::map<std::string, std::set<int>> deniedRanks; // deniedRanks["AI1"] = ranks denied to AI1 by AI2
};

std::string rankToStr(int rank) {
    if (rank == 1) return "Ace";
    if (rank == 11) return "Jack";
    if (rank == 12) return "Queen";
    if (rank == 13) return "King";
    return std::to_string(rank);
}

std::string pluralRank(int rank) {
    if (rank == 1) return "Aces";
    if (rank == 11) return "Jacks";
    if (rank == 12) return "Queens";
    if (rank == 13) return "Kings";
    return std::to_string(rank) + "s";
}

std::string suitToStr(int suit) {
    std::string suits[4] = {"Hearts", "Diamonds", "Clubs", "Spades"};
    return suits[suit];
}

std::string cardToStr(const Card& card) {
    return rankToStr(card.rank) + " of " + suitToStr(card.suit);
}

std::string to_str(int val) {
    return std::to_string(val);
}

void log(const std::string& message) {
    std::cout << message << std::endl;
}

std::string join(const std::vector<Card>& cards, const std::string& delim) {
    std::string s;
    for (size_t i = 0; i < cards.size(); ++i) {
        if (i > 0) s += delim;
        s += cardToStr(cards[i]);
    }
    return s;
}

int countCards(const std::vector<Card>& hand, int r) {
    int cnt = 0;
    for (const auto& c : hand) {
        if (c.rank == r) ++cnt;
    }
    return cnt;
}

int checkBooks(std::vector<Card>& hand, int /*currentBooks*/) {
    int newBooks = 0;
    for (int r = 1; r <= 13; ++r) {
        if (countCards(hand, r) == 4) {
            // Remove the 4 cards of rank r
            hand.erase(std::remove_if(hand.begin(), hand.end(), [r](const Card& c) { return c.rank == r; }), hand.end());
            ++newBooks;
        }
    }
    std::sort(hand.begin(), hand.end());
    return newBooks;
}

State initializeGame() {
    State s;
    std::vector<Card> deck;
    for (int suit = 0; suit < 4; ++suit) {
        for (int rank = 1; rank <= 13; ++rank) {
            deck.push_back({rank, suit});
        }
    }
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(deck.begin(), deck.end(), g);

    // Deal 7 cards each for standard 2-player Go Fish
    s.hand1.assign(deck.begin(), deck.begin() + 7);
    s.hand2.assign(deck.begin() + 7, deck.begin() + 14);
    s.drawPile.assign(deck.begin() + 14, deck.end());

    std::sort(s.hand1.begin(), s.hand1.end());
    std::sort(s.hand2.begin(), s.hand2.end());

    // Check for initial books
    s.books1 += checkBooks(s.hand1, 0);
    s.books2 += checkBooks(s.hand2, 0);

    s.turn = "AI1";
    s.winner = "";
    s.deniedRanks["AI1"] = {};
    s.deniedRanks["AI2"] = {};

    return s;
}

int chooseRank(const std::vector<Card>& hand, const std::set<int>& denied) {
    std::set<int> available;
    for (const auto& c : hand) {
        if (denied.find(c.rank) == denied.end()) {
            available.insert(c.rank);
        }
    }
    if (available.empty()) {
        return 0;
    }
    // Choose the smallest available rank for determinism
    return *available.begin();
}

bool validateRequest(const std::vector<Card>& hand, int r) {
    for (const auto& c : hand) {
        if (c.rank == r) return true;
    }
    return false;
}

std::string processRequest(const std::string& p, const std::string& o, int r,
                           std::vector<Card>& pHand, std::vector<Card>& oHand,
                           int& pBooks, std::vector<Card>& drawPile,
                           std::map<std::string, std::set<int>>& deniedRanks) {
    int num = countCards(oHand, r);
    log(o + ": Yes, " + to_str(num));
    // Transfer cards
    std::vector<Card> transfer;
    for (auto it = oHand.begin(); it != oHand.end(); ) {
        if (it->rank == r) {
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
    pBooks += checkBooks(pHand, pBooks);
    // Clear denied for p since hand changed significantly
    deniedRanks[p].clear();
    // Successful request, go again
    return p;
}

bool noValidMoves(const std::vector<Card>& hand1, const std::vector<Card>& hand2,
                  const std::map<std::string, std::set<int>>& deniedRanks) {
    int r1 = chooseRank(hand1, deniedRanks.at("AI1"));
    int r2 = chooseRank(hand2, deniedRanks.at("AI2"));
    return r1 == 0 && r2 == 0;
}

int main() {
    State s = initializeGame();
    while (s.winner.empty()) {
        // Check end conditions before turn
        if ((s.drawPile.empty() && (s.hand1.empty() || s.hand2.empty())) ||
            (s.hand1.empty() && s.hand2.empty()) ||
            (s.drawPile.empty() && noValidMoves(s.hand1, s.hand2, s.deniedRanks))) {
            if (s.books1 > s.books2) {
                s.winner = "AI1";
            } else if (s.books2 > s.books1) {
                s.winner = "AI2";
            } else {
                s.winner = "Tie";
            }
            continue; // To skip the turn logic and check winner.empty again
        }

        std::string p = s.turn;
        std::string o = (p == "AI1" ? "AI2" : "AI1");
        std::vector<Card>& pHand = (p == "AI1" ? s.hand1 : s.hand2);
        std::vector<Card>& oHand = (p == "AI1" ? s.hand2 : s.hand1);
        int& pBooks = (p == "AI1" ? s.books1 : s.books2);
        if (pHand.empty()) {
            log(p + " hand empty; pass");
            s.turn = o;
        } else {
            std::set<int> denied = s.deniedRanks[p];
            int r = chooseRank(pHand, denied);
            if (r == 0 || !validateRequest(pHand, r)) {
                log("Error: Invalid rank " + rankToStr(r) + " for " + p);
                s.turn = o;
            } else {
                log(p + ": " + o + ", have any " + pluralRank(r) + "?");
                bool hasRank = countCards(oHand, r) > 0;
                if (hasRank) {
                    s.turn = processRequest(p, o, r, pHand, oHand, pBooks, s.drawPile, s.deniedRanks);
                } else {
                    log(o + ": GoFish");
                    s.deniedRanks[p].insert(r);
                    if (!s.drawPile.empty()) {
                        Card card = s.drawPile.front();
                        s.drawPile.erase(s.drawPile.begin());
                        pHand.push_back(card);
                        std::sort(pHand.begin(), pHand.end());
                        log(p + " draws: " + cardToStr(card));
                        pBooks += checkBooks(pHand, pBooks);
                        if (card.rank == r) {
                            log(p + " got the requested rank! Goes again.");
                            s.turn = p;
                        } else {
                            s.turn = o;
                        }
                        // Clear denied after draw
                        s.deniedRanks[p].clear();
                    } else {
                        log("Draw pile empty");
                        s.turn = o;
                    }
                }
            }
        }
    }
    log("Game ended! Winner: " + s.winner + ", Books: " + (s.winner == "Tie" ? to_str(s.books1) : to_str(s.winner == "AI1" ? s.books1 : s.books2)));
    log("Final: {AI1: {hand: [" + join(s.hand1, ", ") + "], books: " + to_str(s.books1) + "}, AI2: {hand: [" + join(s.hand2, ", ") + "], books: " + to_str(s.books2) + "}, drawPile: [" + join(s.drawPile, ", ") + "], turn: " + s.turn + "}");

    return 0;
}
