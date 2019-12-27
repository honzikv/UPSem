//
// Created by itznukee on 11/20/19.
//

#ifndef UPSEM_SERVER_H
#define UPSEM_SERVER_H

#include <iostream>
#include <cstring>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstdlib>
#include <vector>
#include <unistd.h>
#include <cstdio>
#include <sys/select.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <unordered_set>
#include <algorithm>
#include <memory>
#include "Client.h"
#include "lobby/Lobby.h"
#include "communication/Constants.h"
#include "serialization/TCPData.h"
#include "communication/Fields.h"
#include "communication/Values.h"

using namespace std;

class Lobby;

class MessageHandler;

class Server {

        /**
         * Master socket pro select()
         */
        int serverSocketFileDescriptor;

        /**
         * Port serveru
         */
        int port;

        /**
         * Info o adrese serveru
         */
        struct sockaddr_in address;

        /**
         * Info o adrese klienta pro select
         */
        struct sockaddr_in clientAddress;

        /**
         * Set vsech file descriptoru
         */
        fd_set fileDescriptorSet;

        /**
         * Seznam vsech klientu
         */
        vector<shared_ptr<Client>> clients;

        /**
         * Seznam vsech lobby
         */
        vector<shared_ptr<Lobby>> lobbies;

        /**
         * Seznam vsech vlaken
         */
        vector<thread> lobbyThreads;

        vector<int> clientSockets;

        /**
         *
         */
        shared_ptr<MessageHandler> messageHandler;

    public:
        Server(int port, int lobbyCount);

        void init();

        void launch();

        void kickClient(shared_ptr<Client> client);

        shared_ptr<Client> getClient(int socket);

        shared_ptr<Lobby> getLobby(int lobbyId);

        const vector<shared_ptr<Lobby>>& getLobbies();

        bool isLoginUnique(string username);

        bool isLobbyJoinable(int lobbyId);

        void createThreads();

        void addClient(const string& username, int clientSocket);

        void selectServer();
};

#endif