//
// Created by itznukeey on 20.11.19.
//

#ifndef UPSEM_LOBBY_H
#define UPSEM_LOBBY_H

#include <cstdlib>
#include <vector>
#include <memory>
#include "Client.h"
#include "../json-parsing/JSONParser.h"

using namespace std;

class Lobby {

    vector<shared_ptr<Client>> clients;

    shared_ptr<JSONParser> jsonParser;

    bool isFull = false;

    int limit;

public:
    Lobby(const shared_ptr<JSONParser>& jsonParser, int limit);

    void addClient(shared_ptr<Client>& client);

    void sendPlayerCountToClient(int fileDescriptor) {
    }

    void
};


#endif //UPSEM_LOBBY_H
