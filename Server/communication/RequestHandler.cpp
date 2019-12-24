//
// Created by nvk on 12/23/2019.
//

#include "RequestHandler.h"

void RequestHandler::handleSocketRequest(TCPData& message, int& clientSocket) {
    auto request = message.valueOf(REQUEST);

    if (request == LOGIN) {
        MessageWriter::sendUsernameUnique(
                clientSocket, server.isLoginUnique(message.valueOf(USERNAME)));
        return;
    }

}

void RequestHandler::handleClientRequest(TCPData& message, shared_ptr<Client>& client) {
    auto request = message.valueOf(REQUEST);

    if (request == LOBBY_LIST) {
        messageWriter.sendLobbyList(client.operator*());
        return;
    }

    if (request == JOIN_LOBBY) {
        messageWriter.sendLobbyJoinable(client.operator*(), message);
    }
}
