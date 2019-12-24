//
// Created by itznukee on 11/20/19.
//

#include "Client.h"

#include <utility>

Client::Client(string id, int fileDescriptor) : id(std::move(id)), fileDescriptor(fileDescriptor) {}

const string& Client::getId() const {
    return id;
}

int Client::getFileDescriptor() const {
    return fileDescriptor;
}

bool Client::isAuthenticated1() const {
    return isAuthenticated;
}

void Client::setIsAuthenticated(bool authenticated) {
    Client::isAuthenticated = authenticated;
}

bool Client::operator==(const Client& anotherClient) const {
    return id == anotherClient.id &&
           fileDescriptor == anotherClient.fileDescriptor &&
           isAuthenticated == anotherClient.isAuthenticated;
}

bool Client::operator!=(const Client& anotherClient) const {
    return !(anotherClient == *this);
}
