//
// Created by itznukee on 11/20/19.
//

#ifndef UPSEM_SERVER_H
#define UPSEM_SERVER_H


#include <netinet/in.h>
#include <stdlib.h>
#include <vector>
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include "Client.h"

using namespace std;

#define DEFALT_CLIENT_COUNT 5;

class Server {

    int port;

    int fileDescriptor;

    struct sockaddr_in address;

    vector<Client> clients;

public:
    Server(int port);
};


#endif //UPSEM_SERVER_H
