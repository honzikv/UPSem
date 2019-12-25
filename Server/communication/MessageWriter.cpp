//
// Created by nvk on 12/24/2019.
//

#include "MessageWriter.h"
#include "../Server.h"

MessageWriter::MessageWriter(Server& server) : server(server) {}

bool MessageWriter::sendMessage(int clientSocket, const string& message) {
    return send(clientSocket, message.c_str(), message.size(), 0) > 0;
}

void MessageWriter::sendUsernameUnique(int clientSocket, bool isUnique) {
    auto message = TCPData(DATATYPE_RESPONSE);
    message.add(RESPONSE, LOGIN);
    message.add(IS_UNIQUE, isUnique ? TRUE : FALSE);

    sendMessage(clientSocket, message.serialize());
}

void MessageWriter::sendLobbyList(shared_ptr<Client>& client) {
    auto message = TCPData(DATATYPE_RESPONSE);
    message.add(RESPONSE, LOBBY_LIST);
    for (const auto& lobby : server.getLobbies()) {
        message.add("lobby" + to_string(lobby->getId()), lobby->getState());
    }

    sendMessage(client->getClientSocket(), message.serialize());
}

void MessageWriter::sendLobbyJoinable(shared_ptr<Client>& client, bool isJoinable, int lobbyId) {
    auto message = TCPData(DATATYPE_RESPONSE);
    message.add(RESPONSE, JOIN_LOBBY);
    message.add(IS_JOINABLE, isJoinable ? TRUE : FALSE);
    message.add(LOBBY_ID, to_string(lobbyId));

    sendMessage(client->getClientSocket(), message.serialize());
}

void MessageWriter::sendClientNotFound(int clientSocket) {
    auto message = TCPData(DATATYPE_RESPONSE);
    message.add(RESPONSE, RECONNECT);
    message.add(RECONNECTED, FALSE);

    sendMessage(clientSocket, message.serialize());
}

void MessageWriter::sendClientReconnected(int clientSocket) {
    auto message = TCPData(DATATYPE_RESPONSE);
    message.add(RESPONSE, RECONNECT);
    message.add(RECONNECTED, TRUE);

    sendMessage(clientSocket, message.serialize());
}

