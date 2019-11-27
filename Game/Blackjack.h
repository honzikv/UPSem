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

#define TIMEOUT_MS 15000

using namespace std;

class Blackjack {

    const int maxPlayerCount;

    vector<shared_ptr<Player>> players;

    shared_ptr<Player> dealer;

    shared_ptr<Deck> deck;

    shared_ptr<Server> server;

public:
    void addPlayer(shared_ptr<Player> player);

    void start();

    Blackjack(const int maxPlayerCount);
};


#endif //UPSEM_BLACKJACK_H
