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


using namespace std;

class Server {

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

    vector<shared_ptr<Lobby>> lobbies;

    unordered_set<string> clientIds;

public:
    Server(int port, int lobbyCount);

    void run();

    void handleConnection(int socket);

    const vector<shared_ptr<Lobby>>& getLobbies() const;

    void removeClient(shared_ptr<const Client>& client) {
        clientIds.erase(client->getId());
        clients.erase(remove(clients.begin(),clients.end(), client), clients.end());
    }

    int getClientCount() {
        return clients.size();
    }

    bool isLoginUnique(const string& login);

    void addClient(const shared_ptr<Client>& client);
};const

#endif UPSEM_SERVER_H