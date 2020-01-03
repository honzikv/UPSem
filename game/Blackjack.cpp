//
// Created by itznukeey on 27.11.19.
//

#include <iostream>
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

shared_ptr<TurnResult> Blackjack::handleHit(const shared_ptr<Client>& player) {
    if (players[currentPlayerIndex] != player) {
        return make_shared<TurnResult>(RESULT_NOT_YOUR_TURN, nullptr, player);
    }

    auto card = drawCard(player);
    auto playerInfo = player->getPlayerInfo();
    playerInfo->doHit();
    if (playerInfo->isBusted()) {
        moveToNextPlayer();
        return make_shared<TurnResult>(RESULT_BUSTED, card, player);
    }

    return make_shared<TurnResult>(RESULT_OK, card, player);
}

shared_ptr<TurnResult> Blackjack::handleStand(const shared_ptr<Client>& player) {
    if (players[currentPlayerIndex] != player) {
        return make_shared<TurnResult>(RESULT_NOT_YOUR_TURN, nullptr, player);
    }

    moveToNextPlayer();
    return make_shared<TurnResult>(RESULT_OK, nullptr, player);
}

shared_ptr<TurnResult> Blackjack::handleDoubleDown(const shared_ptr<Client>& player) {
    if (players[currentPlayerIndex] != player) {
        return make_shared<TurnResult>(RESULT_NOT_YOUR_TURN, nullptr, player);
    }

    auto playerInfo = player->getPlayerInfo();
    if (playerInfo->hasHit()) {
        return make_shared<TurnResult>(RESULT_HIT_AFTER_DOUBLE, nullptr, player);
    }

    playerInfo->doubleDown();
    auto card = drawCard(player);
    moveToNextPlayer();
    if (playerInfo->isBusted()) {
        return make_shared<TurnResult>(RESULT_DOUBLE_BUSTED, card, player);
    }
    return make_shared<TurnResult>(RESULT_DOUBLE_OK, card, player);
}

void Blackjack::moveToNextPlayer() {
    cout << "moving to next player" << endl;
    currentPlayerIndex++;
    if (currentPlayerIndex == players.size()) {
        dealerPlay();
    }
}

void Blackjack::dealerPlay() {
    //Dokud je ma dealerova ruka hodnotu mensi nez 17 pro mekky soucet bere si dalsi kartu
    cout << "dealer is playing" << endl;
    while (dealer->getPlayerInfo()->getSoftHandValue() < DEALER_MAX_DRAW_SUM) {
        drawCard(dealer);
    }
    gameRunning = false;
}

shared_ptr<Client> Blackjack::getCurrentPlayer() {
    if (isGameRunning()) {
        return players[currentPlayerIndex];
    }
    else return nullptr;
}


void Blackjack::skipPlayer() {
    handleStand(players[currentPlayerIndex]);
}

const shared_ptr<Dealer>& Blackjack::getDealer() const {
    return dealer;
}

