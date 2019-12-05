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

    const int limit;

    const int id;

public:
    int getId() const;

private:

    int isRunning = false;

public:
    Lobby(int limit, int id);

    void addClient(shared_ptr<Client>& client);

    int getClientCount() {
        return clients.size();
    }

    string getState() {
        return to_string(id) + ";" + to_string(getClientCount()) + "/" + to_string(clients.size());
    }
};


#endif //UPSEM_LOBBY_H
