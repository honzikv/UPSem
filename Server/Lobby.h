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
#include "Client.h"
#include "LobbyState.h"
#include "serialization/TCPData.h"
#include "communication/ClientData.h"

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
         * Sockety klientu pro select()
         */
        vector<int> clientSockets;

        /**
         * Flag, zda-li je lobby joinable, tzn. pokud uzivatel zazada o pripojeni do teto lobby, tak bude pripojen ci
         * nikoliv
         */
        bool joinable = false;

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

        /**
         * Namapovaná ID s klienty
         */
        unordered_map<string, shared_ptr<Client>> clientIdsMap;

    public:
        Lobby(int limit, int id);

        int getId() const;

        bool isJoinable() const;

        bool addClient(const shared_ptr<Client>& client);

        int getClientCount();

        string getState();

        void increaseHasVoted();

        bool contains(shared_ptr<Client>& client);

        void addNewMessage(shared_ptr<TCPData>& message, shared_ptr<Client>& client);
};


#endif //UPSEM_LOBBY_H
