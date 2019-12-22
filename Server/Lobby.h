//
// Created by itznukeey on 20.11.19.
//

#ifndef UPSEM_LOBBY_H
#define UPSEM_LOBBY_H
#define MAX_CLIENTS_PER_LOBBY 7

#include <cstdlib>
#include <vector>
#include <memory>
#include "Client.h"

using namespace std;

class Lobby {

    vector<shared_ptr<Client>> clients;

    bool joinable = false;

    const int limit;

    const int id;

public:
    int getId() const;

    bool isJoinable() const;

public:
    Lobby(int limit, int id);

    void addClient(shared_ptr<Client>& client);

    int getClientCount() {
        return clients.size();
    }

    string getState() {
        return to_string(id) + ";" + to_string(getClientCount()) + ";" + to_string(clients.size());
    }

};


#endif //UPSEM_LOBBY_H
