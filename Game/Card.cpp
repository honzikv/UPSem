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


Card::Card(Card::Rank rank, Card::Suit suit) : rank(rank), suit(suit) {
    this->isRevealed = false;
    this->value = rankValues[rank];
}

int Card::getValue() const {
    return value;
}

void Card::setIsRevealed(bool isRevealed) {
    Card::isRevealed = isRevealed;
}

bool Card::isRevealed1() const {
    return isRevealed;
}
