//
// Created by itznukeey on 27.11.19.
//

#ifndef UPSEM_BLACKJACK_H
#define UPSEM_BLACKJACK_H

#include <stdlib.h>
#include <vector>
#include "Player.h"
#include "Deck.h"
#include <memory>

using namespace std;


class Blackjack {

    const int maxPlayerCount;

    vector<shared_ptr<Player>> players;

    shared_ptr<Player> dealer;

    shared_ptr<Deck> deck;

public:
    void addPlayer(shared_ptr<Player> player) {
        players.push_back(player);
    }

    void startRound() {
        auto dealerIndex = rand() % players.size();
        this->dealer = players.at(dealerIndex); //nastavi dealera
        players.erase(players.begin() + dealerIndex); //odstrani dealera z hracu

    }

    Blackjack(const int maxPlayerCount) : maxPlayerCount(maxPlayerCount) {
        this->deck = make_shared<Deck>();
    }
};


#endif //UPSEM_BLACKJACK_H
