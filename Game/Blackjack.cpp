//
// Created by itznukeey on 27.11.19.
//

#include "Blackjack.h"
#include "Dealer.h"

Blackjack::Blackjack(const vector<shared_ptr<Client>>& connectedClients) {
    for (const auto& client : connectedClients) {
        players.push_back(client);
    }

    dealer = make_shared<Dealer>();
    deck = make_unique<Deck>();
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

bool Blackjack::isGameRunning() const {
    return gameRunning;
}

shared_ptr<Card> Blackjack::drawCard(const shared_ptr<Client>& client) {
    auto card = deck->getTop();
    client->getPlayerInfo()->addCard(card);
    return card;
}

void Blackjack::drawCard(const shared_ptr<Dealer>& dealer) {
    dealer->getPlayerInfo()->addCard(deck->getTop());
}

bool Blackjack::contains(const shared_ptr<Client>& player) {
    for (const auto& ingamePlayer : players) {
        if (ingamePlayer == player && ingamePlayer.get() == player.get()) {
            return true;
        }
    }
    return false;
}

shared_ptr<TurnResult> Blackjack::handleHit(const shared_ptr<Client>& player) {
    if (players[currentPlayerIndex] != player) {
        return make_shared<TurnResult>(RESULT_NOT_YOUR_TURN, nullptr, player);
    }

    auto card = drawCard(player);
    auto playerInfo = player->getPlayerInfo();
    if (playerInfo->isBusted()) {
        playerInfo->setFinishedPlaying();
        return make_shared<TurnResult>(RESULT_BUSTED, card, player);
    }

    return make_shared<TurnResult>(RESULT_OK, card, player);
}

shared_ptr<TurnResult> Blackjack::handleStand(const shared_ptr<Client>& player) {
    if (!contains(player)) {
        return make_shared<TurnResult>(RESULT_ERROR, nullptr, player);
    }
    player->getPlayerInfo()->setFinishedPlaying();
    return make_shared<TurnResult>(RESULT_OK, nullptr, player);
}

shared_ptr<Client> Blackjack::getCurrentPlayer() {
    if (!gameRunning) {
        return nullptr;
    }

    return players[currentPlayerIndex];
}

void Blackjack::moveToNextPlayer() {
    /*
     * Pokud jsme na konci vektoru s hraci a vsichni dohrali, hraje dealer
     */
    if (currentPlayerIndex == players.size() - 1 && allPlayersFinished()) {
        dealerPlay();
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

void Blackjack::dealerPlay() {
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

const shared_ptr<Dealer>& Blackjack::getDealer() const {
    return dealer;
}
