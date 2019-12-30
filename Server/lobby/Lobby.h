//
// Created by itznukeey on 20.11.19.
//

#ifndef UPSEM_LOBBY_H
#define UPSEM_LOBBY_H
#define MAX_CLIENTS_PER_LOBBY 7

#include <cstdlib>
#include <vector>
#include <memory>
#include <thread>
#include <algorithm>
#include <atomic>
#include "../Client.h"
#include "LobbyState.h"
#include "../serialization/TCPData.h"
#include "../communication/ClientData.h"
#include "../../Game/Blackjack.h"
#include "GamePreparation.h"

class Server;

class LobbyMessageHandler;

using namespace std;

class Lobby {

        vector<shared_ptr<ClientData>> unprocessedMessages;

        /**
         * Seznam vsech klientu
         */
        vector<shared_ptr<Client>> clients;

        /**
         * Flag, zda-li je lobby joinable, tzn. pokud uzivatel zazada o pripojeni do teto lobby, tak bude pripojen ci
         * nikoliv
         */
        atomic<bool> joinable = true;

        /**
         * Limit hracu
         */
        int limit;

        /**
         * Id lobby, integer >= 0
         */
        const int id;

        /**
         * Pocet uzivatelu, kteri volili pro brzky start hry
         */
        atomic<int> votedToStart = 0;

        LobbyState lobbyState = LOBBY_STATE_WAITING;

        /**
         * Zpracovavani zprav pro instanci lobby
         */
        shared_ptr<LobbyMessageHandler> lobbyMessageHandler;

        unique_ptr<GamePreparation> gamePreparation = nullptr;

        unique_ptr<Blackjack> blackjack;

        chrono::time_point<chrono::system_clock> returnToLobbyStart;

    public:
        /**
         * Vychozi konstruktor, vytvori lobby s limitem hracu a id
         * @param limit limit hracu v lobby
         * @param id unikatni identifikator lobby, cele cislo >= 0
         */
        Lobby(int limit, int id);

        int getId() const;

        LobbyState getLobbyState() const;

        bool isJoinable() const;

        /**
         * Prida shared pointer na instanci klienta do lobby
         * @param client shared pointer klienta
         * @return zda-li se klient pridal
         */
        bool addClient(shared_ptr<Client> client);

        /**
         * Odstrani shared pointer na instanci klienta z lobby
         * @param client reference na shared pointer instanci klienta
         */
        void removeClient(const shared_ptr<Client>& client);

        /**
         * Vrati pocet pripojenych klientu
         * @return  vrati pocet pripojenych klientu
         */
        int getClientCount();

        /**
         * Vytvori string s popisem lobby pro klienta
         * @return string s popisem lobby pro poslani zpravy klientovi
         */
        string toString();

        /**
         * Zvysi pocet hlasu pro start o jedna
         */
        void incrementVotes();

        /**
         * Vrati, zda-li lobby obsahuje klienta
         * @param client klient, ktereho hledame
         * @return true - obsahuje, false - neobsahuje
         */
        bool contains(const shared_ptr<Client>& client);

        /**
         * Prida na frontu novou zpravu ke zpracovani
         * @param message zprava, kterou ma lobby zpracovat
         * @param client klient, ktery zpravu odeslal
         */
        void addNewMessage(const shared_ptr<TCPData>& message, const shared_ptr<Client>& client);

        /**
         * Zkontroluje stav mistnosti, pokud se hraje hra, zkontroluje zda hra stale bezi a adekvatne upravi stav
         */
        void handleLobby();

        /**
         * Vrati seznam klientu v lobby
         * @return vrati seznam vsech klientu v lobby
         */
        const vector<shared_ptr<Client>>& getClients() const;

        /**
         * Zkontroluje, jestli se muze hra hrat
         * @return vrati, zda-li je blackjack hratelny
         */
        bool isPlayable();

        /**
         * Inicializuje pripravu hry
         */
        void initializeGamePrep();

        /**
         * Zpracuje potvrzeni klienta, ze se hry zucastni
         * @param client klient, ktery potvrdil, ze se hry zucastni
         */
        void confirmClient(shared_ptr<Client> client);

        /**
         * Zkontroluje, jestli inicializace skoncila
         * @return true - konec case pro inicializaci, false - je cas pro inicializaci
         */
        bool prepTimeOut();

        /**
         * Odstartuje hru, pokud je playable, jinak oznami klientum, ze se hru nepodarilo spustit
         * @param playable boolean, zda-li je hra hratelna
         */
        void startGame(bool playable);

        /**
         * Vrati klienty po dohrani hry do lobby
         */
        void returnClientsToLobby();

        /**
         *
         * @param client
         * @param message
         */
        void handlePlayerTurn(const shared_ptr<Client>& client, const shared_ptr<TCPData>& message);

        void sendBoardUpdate();

        void endGame();

        void handleGameState();

        void checkIfReturnToLobby();
};


#endif //UPSEM_LOBBY_H
