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
    lastMessageSent = chrono::system_clock::now();
}

bool Blackjack::contains(const shared_ptr<Client>& player) {
    for (const auto& ingamePlayer : players) {
        if (ingamePlayer == player && ingamePlayer.get() == player.get()) {
            return true;
        }
    }
    return false;
}

Result Blackjack::handleHit(const shared_ptr<Client>& player) {
    if (players[currentPlayerIndex] != player) {
        return RESULT_NOT_YOUR_TURN;
    }

    drawCard(player);
    auto playerInfo = player->getPlayerInfo();

    if (playerInfo->isBusted()) {
        playerInfo->setFinishedPlaying();
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

const shared_ptr<Client>& Blackjack::getCurrentPlayer() {
    if (currentPlayerIndex == -1) {
        return dealer;
    }

    return players[currentPlayerIndex];
}

bool Blackjack::isPlayerDealer(const shared_ptr<Client>& player) {
    return dealer == player;
}

void Blackjack::moveToNextPlayer() {
    /*
     * Pokud jsme na konci vektoru s hraci a vsichni dohrali, hraje dealer
     */
    if (currentPlayerIndex == players.size() - 1 && allPlayersFinished()) {
        dealersPlay();
    }

    for (auto i = currentPlayerIndex; i < players.size(); i++) {
        if (players[i]->getPlayerInfo()->isPlaying()) {
            currentPlayerIndex = i;
            break;
        }
    }
}

bool Blackjack::allPlayersFinished() {
    for (const auto& player : players) {
        if (player->getPlayerInfo()->isPlaying()) {
            return false;
        }
    }
    return true;
}

void Blackjack::dealersPlay() {
    //Dokud je ma dealerova ruka hodnotu mensi nez 17 bere si dalsi kartu
    while (dealer->getPlayerInfo()->getHandValue() < DEALER_MAX_DRAW_SUM) {
        drawCard(dealer);
    }
    gameRunning = false;
}

const chrono::time_point<chrono::system_clock>& Blackjack::getLastMessageSent() const {
    return lastMessageSent;
}

void Blackjack::updateLastMessageSent() {
    lastMessageSent = chrono::system_clock::now();
}

void Blackjack::skipPlayer() {
    handleStand(players[currentPlayerIndex]);
}
