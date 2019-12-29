//
// Created by itznukeey on 27.11.19.
//

#include "Deck.h"

Deck::Deck() {
    this->fill();
}

shared_ptr<Card> Deck::getTop() {
    auto top = cards.back();
    cards.pop_back();
    return top;
}

void Deck::fill() {
    for (auto rank = (Card::Rank) 0; rank != Card::Rank::KING; rank = (Card::Rank) (rank + 1)) {
        for (auto suit = (Card::Suit) 0; suit != Card::Suit::HEARTS; suit = (Card::Suit) (suit + 1)) {
            cards.emplace_back(make_shared<Card>(rank, suit));
        }
    }
}

void Deck::shuffle() {
    auto rng = default_random_engine();
    ::shuffle(std::begin(cards), std::end(cards), rng);
}

void Deck::returnCard(shared_ptr<Card>& cardPtr) {
    cards.push_back(cardPtr);
}
