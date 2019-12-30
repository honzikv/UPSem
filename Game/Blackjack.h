//
// Created by itznukeey on 27.11.19.
//

#ifndef UPSEM_BLACKJACK_H
#define UPSEM_BLACKJACK_H

#include <cstdlib>
#include <vector>
#include <memory>
#include "Deck.h"
#include "../Server/Client.h"
#include "Result.h"
#include "Dealer.h"
#include "TurnResult.h"

using namespace std;

class Blackjack {

        vector<shared_ptr<Client>> players;

        shared_ptr<Dealer> dealer;

        unique_ptr<Deck> deck;

        bool gameRunning = true;

        chrono::time_point<chrono::system_clock> lastMessageSent;

        int currentPlayerIndex = 0;

    public:
        explicit Blackjack(const vector<shared_ptr<Client>>& connectedClients);

        const chrono::time_point<chrono::system_clock>& getLastMessageSent() const;

        void updateLastMessageSent();

        void dealCards();

        const shared_ptr<Dealer>& getDealer() const;

        shared_ptr<TurnResult> handleHit(const shared_ptr<Client>& player);

        const vector<shared_ptr<Client>>& getPlayers() const;

        bool isGameRunning() const;

        bool contains(const shared_ptr<Client>& player);

        shared_ptr<Card> drawCard(const shared_ptr<Client>& client);

        shared_ptr<TurnResult> handleStand(const shared_ptr<Client>& player);

        shared_ptr<Client> getCurrentPlayer();

        bool allPlayersFinished();

        void moveToNextPlayer();

        void skipPlayer();

        void dealerPlay();

        void drawCard(const shared_ptr<Dealer>& dealer);
};


#endif //UPSEM_BLACKJACK_H
