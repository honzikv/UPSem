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

string Card::toString() {
    return suitToString() + ";" + rankToString() + ";" + to_string(value);
}

string Card::suitToString() {
    switch (suit) {
        case CLUBS:
            return "clubs";
        case DIAMONDS:
            return "diamonds";
        case SPADES:
            return "spades";
        default:
        case HEARTS:
            return "hearts";
    }
}

string Card::rankToString() {
    switch (rank) {
        case ACE:
            return "ace";
        case TWO:
            return "two";
        case THREE:
            return "three";
        case FOUR:
            return "four";
        case FIVE:
            return "five";
        case SIX:
            return "six";
        case SEVEN:
            return "seven";
        case EIGHT:
            return "eight";
        case NINE:
            return "nine";
        case TEN:
            return "ten";
        case JOKER:
            return "joker";
        case JACK:
            return "jack";
        case QUEEN:
            return "queen";
        default:
        case KING:
            return "king";
    }
}
