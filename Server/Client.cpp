//
// Created by itznukee on 11/20/19.
//

#include "Client.h"

#include <utility>

Client::Client(string id, int fileDescriptor) : username(std::move(id)), clientSocket(fileDescriptor) {}

const string& Client::getUsername() const {
    return username;
}

int Client::getClientSocket() const {
    return clientSocket;
}

bool Client::operator==(const Client& anotherClient) const {
    return username == anotherClient.username &&
           clientSocket == anotherClient.clientSocket;
}

bool Client::operator!=(const Client& anotherClient) const {
    return !(anotherClient == *this);
}

bool Client::hasVoted() const {
    return voted;
}

void Client::setHasVoted(bool hasVoted) {
    Client::voted = hasVoted;
}

void Client::setClientSocket(int clientSocket) {
    Client::clientSocket = clientSocket;
}

bool Client::isInLobby() const {
    return inLobby;
}

void Client::setInLobby(bool inLobby) {
    Client::inLobby = inLobby;
}

int Client::getLobbyId() const {
    return lobbyId;
}

void Client::setLobbyId(int lobbyId) {
    Client::lobbyId = lobbyId;
}
