//
// Created by itznukeey on 20.11.19.
//

#ifndef UPSEM_LOBBY_H
#define UPSEM_LOBBY_H

#include <cstdlib>
#include <vector>
#include <memory>
#include "Client.h"

using namespace std;

class Lobby {

    vector<shared_ptr<Client>> clients;

    bool isFull = false;

    int limit;

public:
    Lobby(int limit);

    void addClient(shared_ptr<Client>& client);

    void sendPlayerCountToClient(int fileDescriptor) {
    }

};


#endif //UPSEM_LOBBY_H
