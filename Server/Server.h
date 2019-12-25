//
// Created by itznukee on 11/20/19.
//

#ifndef UPSEM_SERVER_H
#define UPSEM_SERVER_H

#include <iostream>
#include <cstring>
#include <netinet/in.h>
#include <cstdlib>
#include <vector>
#include <unistd.h>
#include <cstdio>
#include <sys/socket.h>
#include <unordered_set>
#include <algorithm>
#include <memory>
#include "Client.h"
#include "Lobby.h"
#include "communication/Constants.h"
#include "serialization/TCPData.h"
#include "communication/MessageHandler.h"

using namespace std;

class Server {

        /**
         * Master socket pro select()
         */
        int masterSocket;

        /**
         * info o adrese
         */
        sockaddr_in address;

        /**
         * Seznam vsech klientu
         */
        vector<shared_ptr<Client>> clients;

        vector<shared_ptr<Lobby>> lobbies;

        vector<thread> lobbyThreads;

        unique_ptr<MessageHandler> messageHandler;

        unordered_set<string> clientIds;

    public:
        Server(int port, int lobbyCount);

        void run();

        void setMaxSocket(const vector<int>& clientSockets, fd_set& fileDescriptorSet, int& maxSocket) const;

        void kickClient(shared_ptr<Client>& client);

        const shared_ptr<Client>& getClient(int socket);

        const shared_ptr<Lobby>& getLobby(int lobbyId);

        const vector<shared_ptr<Lobby>>& getLobbies() const;

        bool isLoginUnique(string basicString);

        bool isLobbyJoinable(int lobbyId);
};

#endif