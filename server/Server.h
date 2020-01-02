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
#include "client/Client.h"
#include "lobby/Lobby.h"
#include "communication/Constants.h"
#include "communication/serialization/TCPData.h"
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

        /**
         * Vektor s klientskymi sockety pro select
         */
        vector<int> clientSockets;

        /**
         * Set (pokud by byl prikaz zadan vice nez 1, aby se zbytecne nezaviral stejny file descriptor) socketu,
         * ktere se po selectu zavrou
         */
        unordered_set<int> socketsToClose;

        /**
         *
         */
        shared_ptr<MessageHandler> messageHandler;

    public:
        Server(int port, int lobbyCount);

        /**
         * Najde klienta podle id file descriptoru socketu
         * @param socket id socketu
         * @return shared pointer na klienta nebo nullptr, pokud klient nebyl nalezen
         */
        shared_ptr<Client> getClientBySocket(int socket);

        /**
         * Najde a vrati referenci na lobby podle id lobby
         * @param lobbyId cele cislo >=0
         * @return shared pointer na lobby nebo nullptr, pokud lobby nebyla nalezena
         */
        shared_ptr<Lobby> getLobby(int lobbyId);

        /**
         * Najde a vrati lobby, ve ktere
         * @param client
         * @return
         */
        shared_ptr<Lobby> getLobby(const shared_ptr<Client>& client);

        shared_ptr<Client> getClientByUsername(const string& username);

        const vector<shared_ptr<Lobby>>& getLobbies();

        bool isLoginUnique(const string& username);

        bool isLobbyJoinable(int lobbyId);

        void createThreads();

        void registerClient(const string& username, int clientSocket);

        void closeConnection(int clientSocket);

        int setupFileDescriptors(int maxSocket);

        void acceptNewClient(int& addressLength);

        void selectServer();

        void removeClient(const shared_ptr<Client>& client);
};

#endif