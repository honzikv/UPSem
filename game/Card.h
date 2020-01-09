//
// Created by itznukeey on 27.11.19.
//

#ifndef UPSEM_CARD_H
#define UPSEM_CARD_H

#define ACE_SOFT_VALUE 11
#define ACE_HARD_VALUE 1

#include <string>
#include <vector>

using namespace std;

/**
 * Tato trida reprezentuje kartu, jedna se o velmi jednoduchy objekt ktery umi pouze spocitat svou hodnotu ve hre
 * a prevest svoje hodnoty do stringu
 */
class Card {
    public:
        /**
         * Hodnota - Rank karty
         */
        enum Rank {
            ACE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, JACK, QUEEN, KING
        };

        static constexpr initializer_list<Rank> allRanks = {
                ACE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, JACK, QUEEN, KING
        };

        /**
         * Hodnoty enumu z Rank, pouze pro uzitecnost
         */
        static constexpr int rankValues[] = {ACE_HARD_VALUE, 2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 10};

        /**
         * Barva - Suit karty
         */
        enum Suit {
            CLUBS, DIAMONDS, SPADES, HEARTS
        };

        static constexpr initializer_list<Suit> allSuits = {CLUBS, DIAMONDS, SPADES, HEARTS};

    private:
        /**
         * Hodnota karty
         */
        Rank rank;

        /**
         * Barva karty
         */
        Suit suit;

        /**
         * Bodova hodnota karty
         */
        int value;


    public:

        /**
         * Konstruktor pro vytvoreni karty
         * @param rank hodnota karty (A, ONE, TWO, THREE apod.)
         * @param suit barva karty (SPADES, DIAMONDS apod.)
         */
        Card(Rank rank, Suit suit);

        /**
         * Vytvori string hodnoty
         * @return c++ string s hodnotou karty
         */
        string rankToString();

        /**
         * Vrati blackjackovou bodovou hodnotu
         * @return integer s hodnotou instance karty
         */
        int getValue() const;

        /**
         * Getter ranku karty
         * @return rank instance karty
         */
        Rank getRank() const;

        /**
         * Vrati barvu ve stringu
         * @return c++ string s hodnotou barvy
         */
        string suitToString();

        /**
         * Vrati string se vsemi parametry instance
         * @return string se vsemi parametry instance pro serializaci po siti
         */
        string toString();
};


#endif //UPSEM_CARD_H
