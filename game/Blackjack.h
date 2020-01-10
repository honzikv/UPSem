//
// Created by itznukeey on 27.11.19.
//

#ifndef UPSEM_BLACKJACK_H
#define UPSEM_BLACKJACK_H

#include <cstdlib>
#include <vector>
#include <memory>
#include "Deck.h"
#include "../server/client/Client.h"
#include "Result.h"
#include "Dealer.h"
#include "TurnResult.h"

using namespace std;

/**
 * Trida s metody pro hrani blackjacku. Obsahuje vektor vsech hracu, ktery obdrzi od GameControlleru a funkcionalitu
 * pro pokrok cele hry
 */
class Blackjack {

        /**
         * Vektor se vsemi hraci, kteri se zucastni hry
         */
        vector<shared_ptr<Client>> players;

        /**
         * Dealer
         */
        shared_ptr<Dealer> dealer;

        /**
         * Balik s kartami
         */
        unique_ptr<Deck> deck;

        /**
         * Flag, ktery urcuje, zda-li hra bezi, nebo uz skoncila
         */
        bool gameRunning = true;

        /**
         * Aktualni hrac (z vektoru hracu)
         */
        int currentPlayerIndex = 0;

    public:
        /**
         * Konstruktor pro vytvoreni instance Blackjack
         * @param connectedClients vsichni klienti, kteri vsadili - confirmedClients od GameControlleru
         */
        explicit Blackjack(const vector<shared_ptr<Client>>& connectedClients);

        /**
         * Rozda vsem hracum karty
         */
        void dealCards();

        /**
         * Getter pro dealera
         * @return vrati shared pointer na dealera
         */
        const shared_ptr<Dealer>& getDealer() const;

        /**
         * Metoda pro stand hrace
         * @param player hrac, ktery zahral stand
         * @return vysledek po standu
         */
        shared_ptr<TurnResult> handleStand(const shared_ptr<Client>& player);


        /**
         * Metoda pro hit hrace
         * @param player hrac, ktery zahral hit
         * @return vysledek po zahrani hitu
         */
        shared_ptr<TurnResult> handleHit(const shared_ptr<Client>& player);

        /**
         * Metoda pro double down hrace
         * @param player hrac, ktery zahral double down
         * @return vrati vysledek po zahrani double down
         */
        shared_ptr<TurnResult> handleDoubleDown(const shared_ptr<Client>& player);

        /**
         * Getter vsech hracu ve hre
         * @return vrati getter vsech hracu
         */
        const vector<shared_ptr<Client>>& getPlayers() const;

        /**
         * Vrati, zda-li hra porad bezi
         * @return vrati flag, zda-li hra porad bezi
         */
        bool isGameRunning() const;

        /**
         * Prida hraci do ruky jednu kartu
         * @param client klient, ktery si kartu lize
         * @return kartu, kterou si klient liznul
         */
        shared_ptr<Card> drawCard(const shared_ptr<Client>& client);

        /**
         * Vrati aktualne hrajiciho hrace
         * @return shared pointer aktualne hrajiciho hrace
         */
        shared_ptr<Client> getCurrentPlayer();

        /**
         * Presune se na dalsiho hrace
         */
        void moveToNextPlayer();

        /**
         * Preskoci neaktivniho hrace - tzn. zahraje za nej stand
         */
        void skipPlayer();

        /**
         * Zahraje za dealera
         */
        void dealerPlay();

        /**
         * Lizne kartu pro dealera
         * @param dealer reference na dealera
         */
        void drawCard(const shared_ptr<Dealer>& dealer);
};


#endif //UPSEM_BLACKJACK_H
