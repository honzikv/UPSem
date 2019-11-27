//
// Created by itznukeey on 27.11.19.
//

#ifndef UPSEM_CARD_H
#define UPSEM_CARD_H


class Card {

public:
    /**
     * Hodnota - Rank karty
     */
    enum Rank {
        ACE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, JOKER, JACK, QUEEN, KING
    };

    /**
     * Barva - Suit karty
     */
    enum Suit {
        CLUBS, DIAMONDS, SPADES, HEARTS
    };

    Card(Rank rank, Suit suit);

    Rank getRank() const;

    Suit getSuit() const;

    bool isRevealed1() const;

private:
    Rank rank;

    Suit suit;

     bool isRevealed;
};


#endif //UPSEM_CARD_H
