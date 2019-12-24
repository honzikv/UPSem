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
        Lobby(int limit, int id);

        int getId() const;

        bool isJoinable() const;


        bool addClient(shared_ptr<Client>& client);

        int getClientCount();

        string getState();

};


#endif //UPSEM_LOBBY_H
