//
// Created by itznukeey on 27.11.19.
//

#ifndef UPSEM_BLACKJACK_H
#define UPSEM_BLACKJACK_H

#include <stdlib.h>
#include <vector>
#include "Player.h"
#include "Deck.h"
#include "../Server/Server.h"
#include <memory>

using namespace std;

class Blackjack {

    vector<shared_ptr<Player>> players;

    shared_ptr<Player> dealer;

    shared_ptr<Deck> deck;

public:
    void addPlayer(shared_ptr<Player> player);

    void start();

    Blackjack();
};


#endif //UPSEM_BLACKJACK_H
