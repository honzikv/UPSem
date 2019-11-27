//
// Created by itznukeey on 27.11.19.
//

#include "Card.h"

Card::Rank Card::getRank() const {
    return rank;
}

Card::Suit Card::getSuit() const {
    return suit;
}

bool Card::isRevealed1() const {
    return isRevealed;
}

Card::Card(Card::Rank rank, Card::Suit suit) : rank(rank), suit(suit) {
    this->isRevealed = false;
}
