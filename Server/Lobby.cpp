//
// Created by itznukeey on 20.11.19.
//

#include "Lobby.h"

void Lobby::addClient(shared_ptr<Client>& client) {
    clients.push_back(client);
}

Lobby::Lobby(int limit, int id) : limit(limit), id(id) {}

int Lobby::getId() const {
    return id;
}

bool Lobby::isFull() const {
    return full;
}

