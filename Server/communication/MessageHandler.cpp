//
// Created by nvk on 12/24/2019.
//

#include "MessageHandler.h"
#include "../Server.h"

MessageHandler::MessageHandler(Server& server) : server(server) {
    this->messageWriter = make_unique<MessageWriter>(server);
}

void MessageHandler::handleSocketMessage(int clientSocket, TCPData& message) {

    try {
        auto request = message.valueOf(REQUEST);

        if (request == LOGIN) {
            messageWriter->sendUsernameUnique(
                    clientSocket, server.isLoginUnique(message.valueOf(LOGIN)));
            return;
        }

        if (request == RECONNECT) {
            if (!server.isLoginUnique(message.valueOf(LOGIN))) {
                messageWriter->sendClientReconnected(clientSocket);
            } else {
                messageWriter->sendClientNotFound(clientSocket);
            }
        }
    }
    catch (exception&) {
        close(clientSocket);
    }
}

void MessageHandler::handleClientMessage(shared_ptr<Client>& client, TCPData& message) {
    try {
        auto dataTypeFieldValue = message.valueOf(DATA_TYPE);
        if (dataTypeFieldValue == PING) {
            pingBack(client);
        }

        if (dataTypeFieldValue == RESPONSE) {
            handleResponse(client, message);
        } else {
            handleRequest(client, message);
        }
    }
    catch (exception&) {
        server.kickClient(client);
    }
}

void MessageHandler::handleRequest(shared_ptr<Client>& client, TCPData& message) {
    try {
        auto request = message.valueOf(REQUEST);

        if (request == LOBBY_LIST) {
            messageWriter->sendLobbyList(client);
            return;
        }

        if (request == JOIN_LOBBY) {
            auto lobbyId = stoi(message.valueOf(LOBBY_ID));
            auto isJoinable = server.isLobbyJoinable(lobbyId);

            if (isJoinable) {
                server.getLobby(lobbyId)->addClient(client);
                messageWriter->sendLobbyJoinable(client, true, lobbyId);
            }
        }


    }
    catch (exception&) {
        server.kickClient(client);
    }
}

void MessageHandler::handleResponse(shared_ptr<Client>& client, TCPData& data) {

}

void MessageHandler::pingBack(shared_ptr<Client>& sharedPtr) {

}
