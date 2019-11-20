//
// Created by itznukee on 11/20/19.
//

#include <iostream>
#include <cstring>
#include "Server.h"

Server::Server(int port) : port(port) {
    if ((this->fileDescriptor = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        cerr << "Error, OS have not created socket for the server, try again" << endl;
        exit(EXIT_FAILURE);
    }

    memset(&this->address, 0, sizeof(this->address));

    this->address.sin_family = AF_INET;
    this->address.sin_addr.s_addr = INADDR_ANY;
    this->address.sin_port = this->port;

    if (bind(this->fileDescriptor, (struct sockaddr *) &address, sizeof(this->address)) < 0) {
        cerr << "Error, OS "
    }
}
