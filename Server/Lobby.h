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
#include "Client.h"
#include "LobbyState.h"

class Server;
class LobbyMessageHandler;

using namespace std;

class Lobby {

        vector<shared_ptr<Client>> clients;

        bool joinable = false;

        const int limit;

        const int id;

        int votedToStart = 0;

        LobbyState lobbyState = LOBBY_STATE_WAITING;

        unique_ptr<LobbyMessageHandler> lobbyMessageHandler;

    public:
        Lobby(int limit, int id);

        int getId() const;

        bool isJoinable() const;

        bool addClient(shared_ptr<Client>& client);

        int getClientCount();

        string getState();

        void startThread(Lobby& lobby, Server& server);

        void increaseHasVoted();
};


#endif //UPSEM_LOBBY_H
