//
// Created by itznukeey on 21.11.19.
//

#ifndef UPSEM_CLIENTTHREAD_H
#define UPSEM_CLIENTTHREAD_H


#include "Server.h"

class ClientThread {

    /**
     * Reference na server
     */
    Server& server;

    /**
     * Reference na instanci klienta
     */
    Client& client;

    /**
     * Vlakno pro zpracovani pozadavku uzivatele
     */
    thread clientRequests;
public:
    ClientThread(Server &server, Client &client);
};


#endif //UPSEM_CLIENTTHREAD_H
