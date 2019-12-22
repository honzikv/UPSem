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
#include "serialization/TCPData.h"
#include "communication/FieldConstants.h"
#include "communication/Constants.h"
#include "communication/ValueConstants.h"

using namespace std;

class Server {

    /**
     * Filedescriptor socketu
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


    unordered_set<string> clientIds;

public:
    Server(int port, int lobbyCount);

    void run();

    void removeClient(shared_ptr<const Client>& client) {
        clientIds.erase(client->getId());
        clients.erase(remove(clients.begin(), clients.end(), client), clients.end());
    }

    bool isLoginUnique(const string& login);

    shared_ptr<Client> getClient(int socket);

    shared_ptr<Lobby> getLobby(int id);

    bool sendToClient(int socket, const string& data);

    bool sendToClient(Client& client, const string& data);

    void sendLoginUnique(int socket, bool isUnique);

    void sendLobbyList(shared_ptr<Client> sharedPtr);

    void sendLobbyJoinable(Client& client, bool joinable);
};

#endif