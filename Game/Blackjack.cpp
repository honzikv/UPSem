//
// Created by itznukeey on 27.11.19.
//

#include "Blackjack.h"

void Blackjack::addPlayer(shared_ptr<Player> player) {
    players.push_back(player);
}

void Blackjack::start() {
    auto dealerIndex = rand() % players.size();
    this->dealer = players.at(dealerIndex); //nastavi dealera
    players.erase(players.begin() + dealerIndex); //odstrani dealera z hracu


}

Blackjack::Blackjack(const int maxPlayerCount) : maxPlayerCount(maxPlayerCount) {
    this->deck = make_shared<Deck>();
}
