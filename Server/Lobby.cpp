//
// Created by itznukeey on 20.11.19.
//

#include "Lobby.h"

Lobby::Lobby(int limit) : limit(limit) {
}

void Lobby::addClient(Client client) {
    if (this->clients.size() < this->limit) {
        clients.push_back(client);
    }
}
