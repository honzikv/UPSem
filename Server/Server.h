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
#include "Client.h"
#include "Lobby.h"
#include "../json-parsing/JSONParser.h"

#define MESSAGE_BUFFER_BYTES 1024
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
     * vsechny herni mistnosti
     */
    vector<Lobby> lobbies;

    /**
     * Vlakna s
     */
    vector<thread> threadPool;

    /**
     * Seznam vsech klientu
     */
    vector<shared_ptr<Client>> clients;

    JSONParser jsonParser;
public:
    const JSONParser &getJsonParser() const;

public:
    Server(int port, int lobbyCount);

    void run();

    void handleConnection(int socket);

    void addClient(shared_ptr<Client> client);

    string sendAuthenticationRequest(int socket);

    bool isLoginUnique(string login);
};

#endif //UPSEM_SERVER_H
