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
        bool joinable = true;

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
        int votedToStart = 0;

        LobbyState lobbyState = LOBBY_STATE_WAITING;

        /**
         * Zpracovavani zprav pro instanci lobby
         */
        shared_ptr<LobbyMessageHandler> lobbyMessageHandler;

        unordered_map<string, chrono::time_point<chrono::system_clock>> clientResponseTimes;

        /**
         * Cas, kdy lobby naposledy odeslala zadost klientum, aby si aktualizovali player list
         */
        chrono::time_point<chrono::system_clock> lastPlayerListUpdate;

    public:
        Lobby(int limit, int id);

        int getId() const;

        bool isJoinable() const;

        bool addClient(const shared_ptr<Client>& client);

        void removeClient(const shared_ptr<Client>& client);

        int getClientCount();

        string getState();

        void incrementVotes();

        bool contains(const shared_ptr<Client>& client);

        void addNewMessage(const shared_ptr<TCPData>& message, const shared_ptr<Client>& client);

        void processMessages();

        const vector<shared_ptr<Client>>& getClients() const;

        void updatePlayerResponseTime(const string& username);

        bool isTimeToPlay();

        void startGame();

        void handleGameState();
};


#endif //UPSEM_LOBBY_H
