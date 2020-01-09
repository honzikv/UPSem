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

/**
 * Trida reprezentujici herni balik, ma vektor karet, ktere vraci pomoci metody {@code getTop()}
 */
class Deck {

        /**
         * Vektor karet
         */
        vector<shared_ptr<Card>> cards;

        /**
         * Naplni vektor karet vsemi kartami
         */
        void fill();

    public:
        /**
         * Konstruktor pro vytvoreni decku, balik se po vytvoreni sam zamicha a je pripraven ke hrani
         */
        Deck();

        /**
         * Odstrani vrchni kartu v baliku
         * @return vrati shared pointer na odstranenou kartu
         */
        shared_ptr<Card> getTop();

        /**
         * Zamicha karty
         */
        void shuffle();

};


#endif //UPSEM_DECK_H
