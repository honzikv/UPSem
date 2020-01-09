//
// Created by itznukee on 11/20/19.
//

#include "Client.h"


Client::Client(string username, int clientSocket) : username(move(username)), clientSocket(clientSocket) {
    lastMessageReceived = chrono::system_clock::now();
}

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

void Client::setReady(bool isReady) {
    Client::ready = isReady;
}

void Client::setClientSocket(int clientSocket) {
    Client::clientSocket = clientSocket;
}

int Client::getLobbyId() const {
    return lobbyId;
}

void Client::setLobbyId(int lobbyId) {
    Client::lobbyId = lobbyId;
}

void Client::updateLastMessageReceived() {
    lastMessageReceived = chrono::system_clock::now();
}

const chrono::time_point<chrono::system_clock>& Client::getLastMessageReceived() const {
    return lastMessageReceived;
}

const shared_ptr<PlayerInfo>& Client::getPlayerInfo() const {
    return playerInfo;
}

void Client::createPlayerInfo(int bet) {
    playerInfo = make_shared<PlayerInfo>(bet);
}

string Client::toString() {
    auto isReady = ready ? " READY" : " NOT READY";
    return string(getUsername() + isReady);
}

bool Client::isReady() const {
    return ready;
}


