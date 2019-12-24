//
// Created by nvk on 12/23/2019.
//

#include "MessageWriter.h"

bool MessageWriter::sendMessage(int clientSocket, const string& message) {
    return send(clientSocket, message.c_str(), message.size(), 0) > 0;
}

void MessageWriter::sendUsernameUnique(int clientSocket, bool isUnique) {
    auto message = TCPData(DATATYPE_RESPONSE);
    message.add(RESPONSE, LOGIN);
    message.add(IS_UNIQUE, isUnique ? TRUE : FALSE);
    sendMessage(clientSocket, message.serialize());
}

void MessageWriter::sendLobbyList(Client& client) {
    auto message = TCPData(DATATYPE_RESPONSE);
    message.add(RESPONSE, LOBBY_LIST);
    for (const auto& lobby : server.getLobbies()) {
        message.add("lobby" + to_string(lobby->getId()), lobby->getState());
    }

    sendMessage(client.getFileDescriptor(), message.serialize());
}

void MessageWriter::sendLobbyJoinable(Client& client, TCPData& data) {
    auto lobbyId = stoi(data.valueOf(LOBBY_ID));

    auto lobby = server.getLobby(lobbyId);

    if (lobby != nullptr) {

        if (lobby->isJoinable()) {
            lobby->addClient(client);
        }

        auto message = TCPData(DATATYPE_RESPONSE);
        message.add(RESPONSE, JOIN_LOBBY);
        message.add(IS_JOINABLE, lobby->isJoinable() ? TRUE : FALSE);
    } else {
        server.kickClient(client);
    }
}
