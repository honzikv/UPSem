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

class Deck {

        vector<shared_ptr<Card>> cards;

    public:
        Deck();

        shared_ptr<Card> getTop();

        void shuffle();

        void returnCard(shared_ptr<Card>& cardPtr);

        int cardsLeft();

    private:
        void fill();

};


#endif //UPSEM_DECK_H
