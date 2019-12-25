//
// Created by itznukeey on 20.11.19.
//

#include "Lobby.h"
#include "LobbyMessageHandler.h"

bool Lobby::addClient(const shared_ptr<Client>& client) {

    if (find(clients.begin(), clients.end(), client) != clients.end()) {
        return false;
    }

    clients.push_back(client);
    clientIdsMap[client->getId()] = client;

    return true;
}

Lobby::Lobby(int limit, int id) : limit(limit), id(id) {
    this->lobbyMessageHandler = make_shared<LobbyMessageHandler>(*this);
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

void Lobby::increaseHasVoted() {
    Lobby::votedToStart += 1;
}

int Lobby::getId() const {
    return id;
}

bool Lobby::contains(shared_ptr<Client>& client) {
    return clientIdsMap.find(client->getId()) != clientIdsMap.end();
}

void Lobby::addNewMessage(shared_ptr<TCPData>& message, shared_ptr<Client>& client) {
    unprocessedMessages.emplace_back(make_shared<ClientData>(message, client));
}
