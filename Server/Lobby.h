//
// Created by itznukeey on 20.11.19.
//

#ifndef UPSEM_LOBBY_H
#define UPSEM_LOBBY_H

#include <stdlib.h>
#include <vector>
#include <unordered_set>
#include "Client.h"

using namespace std;

class Lobby {

    vector<Client> clients;

    bool isFull = false;

    int limit;

public:
    Lobby(int limit);

public:
    void addClient(Client client);
};


#endif //UPSEM_LOBBY_H
