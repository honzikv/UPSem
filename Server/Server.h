//
// Created by itznukee on 11/20/19.
//

#ifndef UPSEM_SERVER_H
#define UPSEM_SERVER_H

#include <iostream>
#include <cstring>
#include <thread>
#include <netinet/in.h>
#include <cstdlib>
#include <vector>
#include <unistd.h>
#include <cstdio>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <map>
#include <unordered_set>
#include <algorithm>
#include "Client.h"
#include "Lobby.h"

#define DEFAULT_LOGIN_TIMEOUT_SECONDS 60

using namespace std;

class Server {

    /**
     * Pocet hernich mistnosti na serveru
     */
    int lobbyCount;

    /**
     * Filedescriptor socketu
     */
    int fileDescriptor;

    /**
     * delka adresy
     */
    int addressLength;

    /**
     * info o adrese
     */
    sockaddr_in address;

    /**
     * Seznam vsech klientu
     */
    vector<shared_ptr<Client>> clients;

    unordered_set<string> clientIds;

public:
    Server(int port, int lobbyCount);

    void run();

    void handleConnection(int socket);

    void addClient(shared_ptr<Client> client);

    void removeClient(shared_ptr<Client>& client) {
        clientIds.erase(client->getId());
        clients.erase(remove(clients.begin(),clients.end(), client), clients.end());
    }

    int getClientCount() {
        return clients.size();
    }

    bool isLoginUnique(string login);
};

#endif //UPSEM_SERVER_H
