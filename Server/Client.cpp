//
// Created by itznukee on 11/20/19.
//

#include "Client.h"

#include <utility>

Client::Client(string id, int fileDescriptor) : id(std::move(id)), clientSocket(fileDescriptor) {}

const string& Client::getId() const {
    return id;
}

int Client::getClientSocket() const {
    return clientSocket;
}

bool Client::operator==(const Client& anotherClient) const {
    return id == anotherClient.id &&
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
