//
// Created by itznukeey on 20.11.19.
//

#include "Lobby.h"
#include "Server.h"
#include "LobbyMessageHandler.h"

bool Lobby::addClient(shared_ptr <Client> &client) {

    if (find(clients.begin(), clients.end(), client) != clients.end()) {
        return false;
    }

    clients.push_back(client);
    return true;
}

Lobby::Lobby(int limit, int id) : limit(limit), id(id) {
    this->lobbyMessageHandler = make_unique<LobbyMessageHandler>(*this);
}

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

void Lobby::startThread(Lobby& lobby, Server& server) {
    auto lobbyThread = thread([lobby]{
        while (true) {
            //todo select()
        }
    });
}

void Lobby::increaseHasVoted() {
    Lobby::votedToStart += 1;
}
