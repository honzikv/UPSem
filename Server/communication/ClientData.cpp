//
// Created by nvk on 12/25/2019.
//

#include "ClientData.h"

#include <utility>

ClientData::ClientData(shared_ptr<TCPData> message, shared_ptr<Client> client)
        : message(std::move(message)), client(std::move(client)) {}

const shared_ptr<TCPData>& ClientData::getMessage() const {
    return message;
}

const shared_ptr<Client>& ClientData::getClient() const {
    return client;
}
