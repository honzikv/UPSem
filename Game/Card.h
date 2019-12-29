//
// Created by itznukeey on 27.11.19.
//

#ifndef UPSEM_CARD_H
#define UPSEM_CARD_H

#include <string>

using namespace std;

class Card {


    public:
        /**
         * Hodnota - Rank karty
         */
        enum Rank {
            ACE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, JOKER, JACK, QUEEN, KING
        };

        /**
         * Hodnoty enumu z Rank, pouze pro uzitecnost
         */
        static constexpr int rankValues[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 10};

        /**
         * Barva - Suit karty
         */
        enum Suit {
            CLUBS, DIAMONDS, SPADES, HEARTS
        };

    private:
        Rank rank;

        Suit suit;

        int value;

        bool isRevealed;


    public:

        Card(Rank rank, Suit suit);

        Rank getRank() const;

        Suit getSuit() const;

        void setIsRevealed(bool isRevealed);

        string rankToString();

        int getValue() const;

        string suitToString();

        string toString();
};


#endif //UPSEM_CARD_H
