//
// Created by itznukeey on 27.11.19.
//

#include "Blackjack.h"

Blackjack::Blackjack(const vector<shared_ptr<Client>>& connectedClients) {
    for (const auto& client : connectedClients) {
        players.push_back(client);
    }

    deck = make_unique<Deck>();
    dealer = selectDealer();
    players.erase(remove(players.begin(), players.end(), dealer), players.end());
}

shared_ptr<Client> Blackjack::selectDealer() {
    auto randomDevice = random_device();
    mt19937 engine{randomDevice()};
    uniform_int_distribution<int> dist(0, players.size() - 1);
    return players[dist(engine)];
}

void Blackjack::dealCards() {
    for (const auto& player : players) {
        player->clearPlayerInfo();
        auto playerInfo = player->getPlayerInfo();

        //Pripravi hraci dve karty
        playerInfo->addCard(deck->getTop());
        playerInfo->addCard(deck->getTop());
    }

    auto dealerInfo = dealer->getPlayerInfo();
    dealerInfo->addCard(deck->getTop());
    dealerInfo->addCard(deck->getTop());
}

const vector<shared_ptr<Client>>& Blackjack::getPlayers() const {
    return players;
}

const shared_ptr<Client>& Blackjack::getDealer() const {
    return dealer;
}

bool Blackjack::isGameRunning() const {
    return gameRunning;
}

void Blackjack::drawCard(const shared_ptr<Client>& client) {
    client->getPlayerInfo()->addCard(deck->getTop());
}

void Blackjack::setGameStartNow() {
    gameStart = chrono::system_clock::now();
}

bool Blackjack::contains(const shared_ptr<Client>& player) {
    for (const auto& ingamePlayer : players) {
        if (ingamePlayer == player) {
            return true;
        }
    }
    return false;
}

Result Blackjack::handleHit(const shared_ptr<Client>& player) {
    if (!contains(player)) {
        return RESULT_ERROR;
    }

    drawCard(player);
    auto playerInfo = player->getPlayerInfo();

    if (playerInfo->isBusted()) {
        return RESULT_BUSTED;
    }
    return RESULT_OK;
}

Result Blackjack::handleStand(const shared_ptr<Client>& player) {
    if (!contains(player)) {
        return RESULT_ERROR;
    }

    auto playerInfo = player->getPlayerInfo();
    if (playerInfo->isBusted()) {
        return RESULT_STAND_BUSTED;
    }

    playerInfo->setFinishedPlaying();
    return RESULT_OK;

}
