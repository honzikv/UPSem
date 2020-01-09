//
// Created by itznukeey on 27.11.19.
//

#include "Deck.h"

Deck::Deck() {
    this->fill();
    this->shuffle();
}

shared_ptr<Card> Deck::getTop() {
    auto top = cards.back();
    cards.pop_back();
    return top;
}

void Deck::fill() {
    auto allRanks = Card::allRanks;
    auto allSuits = Card::allSuits;

    for (auto suit : allSuits) {
        for (auto rank : allRanks) {
            cards.emplace_back(make_shared<Card>(rank, suit));
        }
    }
}

void Deck::shuffle() {
    ::shuffle(cards.begin(), cards.end(), random_device());
}

