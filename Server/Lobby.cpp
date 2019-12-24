//
// Created by itznukeey on 20.11.19.
//

#include "Lobby.h"
#include <algorithm>

bool Lobby::addClient(shared_ptr<Client>& client) {

    if (find(clients.begin(),clients.end(), client) != clients.end()) {
        return false;
    }

    clients.push_back(client);
    return true;
}

Lobby::Lobby(int limit, int id) : limit(limit), id(id) {}

int Lobby::getId() const {
    return id;
}

bool Lobby::isJoinable() const {
    return joinable;
}

string Lobby::getState() {
    return to_string(id) + ";" + to_string(getClientCount()) + ";" + to_string(limit);
}

int Lobby::getClientCount() {
    return clients.size();
}


