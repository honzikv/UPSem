//
// Created by itznukeey on 27.11.19.
//

#include "Card.h"

Card::Card(Card::Rank rank, Card::Suit suit) : rank(rank), suit(suit) {
    this->value = rankValues[rank];
}

int Card::getValue() const {
    return value;
}

string Card::toString() {
    return suitToString() + ";" + rankToString();
}

string Card::suitToString() {
    switch (suit) {
        case CLUBS:
            return "C";
        case DIAMONDS:
            return "D";
        case SPADES:
            return "S";
        default:
        case HEARTS:
            return "H";
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
        case JACK:
            return "jack";
        case QUEEN:
            return "queen";
        default:
        case KING:
            return "king";
    }
}
