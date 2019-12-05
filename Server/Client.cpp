//
// Created by itznukee on 11/20/19.
//

#include "Client.h"

#include <utility>

Client::Client(string id, int fileDescriptor) : id(std::move(id)), fileDescriptor(fileDescriptor) {}

const string &Client::getId() const {
    return id;
}

int Client::getFileDescriptor() const {
    return fileDescriptor;
}
