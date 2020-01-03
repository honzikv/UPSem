//
// Created by itznukeey on 20.11.19.
//

#ifndef UPSEM_LOBBY_H
#define UPSEM_LOBBY_H
#define MAX_CLIENTS_PER_LOBBY 6

#include <cstdlib>
#include <vector>
#include <memory>
#include <thread>
#include <algorithm>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <queue>
#include "../client/Client.h"
#include "LobbyState.h"
#include "../communication/serialization/TCPData.h"
#include "../client/ClientData.h"
#include "../../Game/Blackjack.h"
#include "GamePreparation.h"

using namespace std;

class Server;

class LobbyMessageHandler;

class GameController;

class Lobby {

        /**
         * Nezpracovane zpravy
         */
        queue<shared_ptr<ClientData>> unprocessedMessages;

        /**
         * Seznam vsech klientu
         */
        vector<shared_ptr<Client>> clients;

        /**
         * Limit hracu
         */
        const int limit;

        /**
         * Id lobby, integer >= 0
         */
        const int id;

        /**
         * Pocet uzivatelu, kteri volili pro brzky start hry
         */
        int clientsReady = 0;

        LobbyState lobbyState = LOBBY_STATE_WAITING;

        /**
         * Zpracovavani zprav pro instanci lobby
         */
        shared_ptr<LobbyMessageHandler> lobbyMessageHandler;

        shared_ptr<GameController> gameController = nullptr;

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

        void setLobbyState(LobbyState lobbyState);

        void restoreState(const shared_ptr<Client>& client, TCPData& request);

        bool isJoinable() const;

        /**
         * Prida shared pointer na instanci klienta do lobby
         * @param client shared pointer klienta
         * @return zda-li se klient pridal
         */
        void addClient(shared_ptr<Client> client);

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
         * Zvysi pocet hracu, kteri jsou pripraveni na hru
         */
        void incrementPlayersReady();

        void resetClientParticipation();

        /**
         * Vrati, zda-li lobby obsahuje klienta
         * @param client klient, ktereho hledame
         * @return true - obsahuje, false - neobsahuje
         */
        bool contains(const shared_ptr<Client>& client);

        bool hasClientConfirmedToPlay(const shared_ptr<Client>& client);

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

        void sendClientDidntConfirm(const shared_ptr<Client>& client);

        bool reconnectClient(const shared_ptr<Client>& client);

        void sendClientDisconnected(const shared_ptr<Client>& client);

        void checkIfReturnToLobby();

        const shared_ptr<GameController>& getGameController() const;

        void prepareToReturn();

        bool canPrepareGameStart();
};


#endif //UPSEM_LOBBY_H
