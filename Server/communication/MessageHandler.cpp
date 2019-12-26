//
// Created by nvk on 12/26/2019.
//

#include "MessageHandler.h"
#include "Values.h"
#include "Fields.h"
#include "../Server.h"

MessageHandler::MessageHandler(Server& server) : server(server) {}

void MessageHandler::handleSocketMessage(int clientSocket, const shared_ptr<TCPData>& message) {
    try {
        auto request = message->valueOf(REQUEST);

        if (request == LOGIN) {
            if (server.isLoginUnique(message->valueOf(USERNAME))) {
                server.addClient(message->valueOf(USERNAME), clientSocket);
                sendUsernameUnique(clientSocket, true);
            }
            sendUsernameUnique(clientSocket, false);
            return;
        }

        if (request == RECONNECT) {
            if (!server.isLoginUnique(message->valueOf(LOGIN))) {
                //todo actually reconnect the fucker
                sendClientReconnected(clientSocket);
            } else {
                sendClientNotFound(clientSocket);
            }
        }
    }
    catch (exception& ex) {
        cerr << ex.what();
        throw exception();
    }
}

void MessageHandler::handleClientMessage(const shared_ptr<Client>& client, const shared_ptr<TCPData>& message) {

    try {
        auto dataTypeFieldValue = message->valueOf(DATA_TYPE);
        if (dataTypeFieldValue == PING) {
            pingBack(client);
        }

        if (dataTypeFieldValue == RESPONSE) {
            handleClientResponse(client, message);
        } else {
            handleClientRequest(client, message);
        }
    }
    catch (exception&) {
        server.kickClient(client);
    }
}

void MessageHandler::handleClientRequest(const shared_ptr<Client>& client, const shared_ptr<TCPData>& message) {
    try {
        auto request = message->valueOf(REQUEST);

        if (request == LOBBY_LIST) {
            sendLobbyList(client);
            return;
        }

        if (request == JOIN_LOBBY) {
            auto lobbyId = stoi(message->valueOf(LOBBY_ID));
            auto isJoinable = server.isLobbyJoinable(lobbyId);

            if (isJoinable) {
                server.getLobby(lobbyId)->addClient(client);
                sendLobbyJoinable(client, true, lobbyId);
            }
        }
    }
    catch (exception&) {
        server.kickClient(client);
    }
}

void MessageHandler::sendMessage(int socket, const string& message) {
    send(socket, message.c_str(), message.size(), 0);
}

void MessageHandler::handleClientResponse(const shared_ptr<Client>& client, const shared_ptr<TCPData>& message) {

}

void MessageHandler::pingBack(const shared_ptr<Client>& client) {
    auto message = TCPData(DATATYPE_RESPONSE);
    message.add(RESPONSE, PING);
    sendMessage(client->getClientSocket(), message.serialize());
}

void MessageHandler::sendLobbyJoinable(const shared_ptr<Client>& client, bool joinable, int lobbyId) {
    auto message = TCPData(DATATYPE_RESPONSE);
    message.add(RESPONSE, JOIN_LOBBY);
    message.add(IS_JOINABLE, joinable ? TRUE : FALSE);
    message.add(LOBBY_ID, to_string(lobbyId));

    sendMessage(client->getClientSocket(), message.serialize());
}

void MessageHandler::sendUsernameUnique(int clientSocket, bool isUnique) {
    auto message = TCPData(DATATYPE_RESPONSE);
    message.add(RESPONSE, LOGIN);
    message.add(IS_UNIQUE, isUnique ? TRUE : FALSE);

    sendMessage(clientSocket, message.serialize());
}

void MessageHandler::sendLobbyList(const shared_ptr<Client>& client) {
    auto message = TCPData(DATATYPE_RESPONSE);
    message.add(RESPONSE, LOBBY_LIST);
    for (const auto& lobby : server.getLobbies()) {
        message.add("lobby" + to_string(lobby->getId()), lobby->getState());
    }

    sendMessage(client->getClientSocket(), message.serialize());
}

void MessageHandler::sendClientNotFound(int clientSocket) {
    auto message = TCPData(DATATYPE_RESPONSE);
    message.add(RESPONSE, RECONNECT);
    message.add(RECONNECTED, FALSE);

    sendMessage(clientSocket, message.serialize());
}

void MessageHandler::sendClientReconnected(int clientSocket) {
    auto message = TCPData(DATATYPE_RESPONSE);
    message.add(RESPONSE, RECONNECT);
    message.add(RECONNECTED, TRUE);

    sendMessage(clientSocket, message.serialize());
}
