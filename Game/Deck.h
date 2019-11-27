//
// Created by itznukeey on 27.11.19.
//

#ifndef UPSEM_DECK_H
#define UPSEM_DECK_H

#include <stdlib.h>
#include <random>
#include <vector>
#include <algorithm>
#include <memory>
#include "Card.h"

using namespace std;

class Deck {

    vector<shared_ptr<Card>> cards;

public:
    Deck() {
        this->fill();
    }

    shared_ptr<Card> getTop() {
        auto top = cards.back();
        cards.pop_back();
        return top;
    }

private:
    void fill() {
        for (auto rank = (Card::Rank) 0; rank != Card::Rank::KING; rank = (Card::Rank) (rank + 1)) {
            for (auto suit = (Card::Suit) 0; suit != Card::Suit::HEARTS; suit = (Card::Suit) (suit + 1)) {
                cards.push_back(make_shared<Card>(new Card(rank, suit)));
            }
        }
    }

    void shuffle() {
        auto rng = std::default_random_engine{};
        ::shuffle(std::begin(cards), std::end(cards), rng);
    }
};


#endif //UPSEM_DECK_H
