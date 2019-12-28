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
    clientIdsMap[client->getUsername()] = client;

    if (clients.size() == limit) {
        joinable = false;
    }

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

bool Lobby::contains(const shared_ptr<Client>& client) {
    return clientIdsMap.find(client->getUsername()) != clientIdsMap.end();
}

void Lobby::addNewMessage(const shared_ptr<TCPData>& message, const shared_ptr<Client>& client) {
    unprocessedMessages.emplace_back(make_shared<ClientData>(message, client));
}

const vector<shared_ptr<Client>>& Lobby::getClients() const {
    return clients;
}

void Lobby::removeClient(const shared_ptr<Client>& client) {
    if (clientIdsMap.erase(client->getUsername()) == 0 && !joinable) {
        joinable = true;
    }
    clients.erase(remove(clients.begin(),clients.end(),client),clients.end());
}
