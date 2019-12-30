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

using namespace std;

class Blackjack {

        vector<shared_ptr<Client>> players;

        shared_ptr<Client> dealer;

        unique_ptr<Deck> deck;

        bool gameRunning = true;

        chrono::time_point<chrono::system_clock> lastMessageSent;

        int currentPlayerIndex = 0;

    public:
        explicit Blackjack(const vector<shared_ptr<Client>>& connectedClients);

        const chrono::time_point<chrono::system_clock>& getLastMessageSent() const;

        void updateLastMessageSent();

        shared_ptr<Client> selectDealer();

        void dealCards();

        Result handleHit(const shared_ptr<Client>& player);

        const vector<shared_ptr<Client>>& getPlayers() const;

        const shared_ptr<Client>& getDealer() const;

        bool isGameRunning() const;

        void setGameStartNow();

        bool contains(const shared_ptr<Client>& player);

        void drawCard(const shared_ptr<Client>& client);

        Result handleStand(const shared_ptr<Client>& player);

        const shared_ptr<Client>& getCurrentPlayer();

        bool isPlayerDealer(const shared_ptr<Client>& player);

        bool allPlayersFinished();

        void moveToNextPlayer();

        void skipPlayer();

        void dealersPlay();
};


#endif //UPSEM_BLACKJACK_H
