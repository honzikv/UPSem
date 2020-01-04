//
// Created by nvk on 12/26/2019.
//

#include "MessageHandler.h"
#include "../../lobby/controller/GameController.h"
#include "../Values.h"
#include "../Fields.h"
#include "../../Server.h"

MessageHandler::MessageHandler(Server& server) : server(server) {}

void MessageHandler::sendMessage(int socket, const string& message) {
    if (send(socket, message.c_str(), message.size(), 0) <= 0) {
        server.closeConnection(socket);
    }
}

void MessageHandler::handleMessage(int clientSocket, const shared_ptr<TCPData>& message) {
    auto dataType = message->valueOf(DATATYPE);
    if (dataType == REQUEST) {
        handleRequest(clientSocket, message);
    } else if (dataType == RESPONSE) {
        handleResponse(clientSocket, message);
    } else if (dataType == PING) {
        pingBack(clientSocket);
    }
}

void MessageHandler::sendLobbyList(const shared_ptr<Client>& client) {
    auto message = TCPData(DATATYPE_RESPONSE);
    message.add(RESPONSE, LOBBY_LIST);
    for (const auto& lobby : server.getLobbies()) {
        message.add("lobby" + to_string(lobby->getId()), lobby->toString());
    }

    sendMessage(client->getClientSocket(), message.serialize());
}

void MessageHandler::pingBack(int clientSocket) {
    auto message = TCPData(DATATYPE_RESPONSE);
    message.add(RESPONSE, PING);
    sendMessage(clientSocket, message.serialize());

    auto client = server.getClientBySocket(clientSocket);
    client->updateLastMessageReceived();
}

void MessageHandler::handleRequest(int clientSocket, const shared_ptr<TCPData>& message) {
    auto request = message->valueOf(REQUEST);
    if (request == LOGIN) {
        handleLogin(clientSocket, message);
    } else {
        auto client = server.getClientBySocket(clientSocket);

        if (client == nullptr) {
            cout << "Closing unauthenticated socket" << endl;
            server.closeConnection(clientSocket);
            return;
        }

        if (request == LOBBY_LIST) {
            //posle lobby list
            sendLobbyList(client);
        } else if (request == JOIN_LOBBY) {
            //posle info o lobby, pokud je lobby plna posle pouze ze je plna, jinak posle i jmena hracu
            sendLobby(client, message);
        } else if (request == LEAVE_LOBBY) {
            leaveLobby(client, message);
        } else if (client->getLobbyId() != -1) {
            auto lobby = server.getLobby(client->getLobbyId());
            if (lobby != nullptr) {
                lobby->addNewMessage(message, client);
            }
        }

        client->updateLastMessageReceived();
    }
}

void MessageHandler::handleLogin(int clientSocket, const shared_ptr<TCPData>& message) {
    auto username = message->valueOf(USERNAME);
    auto client = server.getClientByUsername(username);

    //Pokud instance klienta k loginu neexistuje vytvorime novy
    if (client == nullptr) {
        server.registerClient(username, clientSocket);
        sendLoginIsNew(clientSocket);
        cout << "Welcome \"" << username << "\" on the server " << endl;
    } else {
        //Jinak odpojime predchozi pripojeni (pokud nebylo odpojeno)
        client = this->server.getClientByUsername(username);
        if (client->getClientSocket() != -1) {
            sendClientReconnectedFromAnotherLocation(client->getClientSocket());
            server.closeConnection(client->getClientSocket());
        }
        client->setClientSocket(clientSocket);
        restoreClientState(client);
        client->updateLastMessageReceived();
        cout << "Client \"" << username << "\" was reconnected " << endl;
    }
}

void MessageHandler::sendLoginIsNew(int clientSocket) {
    auto message = TCPData(DATATYPE_RESPONSE);
    message.add(RESPONSE, LOGIN);
    message.add(RESTORE_STATE, FALSE);
    sendMessage(clientSocket, message.serialize());
}

void MessageHandler::handleResponse(int clientSocket, const shared_ptr<TCPData>& message) {
    auto response = message->valueOf(RESPONSE);
    auto client = server.getClientBySocket(clientSocket);

    if (client == nullptr) {
        cout << "Closing unauthenticated socket" << endl;
        server.closeConnection(clientSocket);
        return;
    }

    auto lobby = server.getLobby(client->getLobbyId());

    if (lobby == nullptr) {
        cout << "Closing incorrect response" << endl;
        server.closeConnection(client->getClientSocket());
    } else {
        lobby->addNewMessage(message, client);
    }

    client->updateLastMessageReceived();
}

void MessageHandler::sendLobby(shared_ptr<Client>& client, const shared_ptr<TCPData>& message) {
    auto response = TCPData(DATATYPE_RESPONSE);
    response.add(RESPONSE, JOIN_LOBBY);
    auto lobbyId = stoi(message->valueOf(LOBBY_ID));
    auto isJoinable = server.isLobbyJoinable(lobbyId);
    response.add(IS_JOINABLE, isJoinable ? TRUE : FALSE);

    sendMessage(client->getClientSocket(), response.serialize());

    if (isJoinable) {
        auto lobby = server.getLobby(lobbyId);
        lobby->addClient(client);
    }
}

void MessageHandler::leaveLobby(shared_ptr<Client>& client, const shared_ptr<TCPData>& message) {
    auto lobby = server.getLobby(client->getLobbyId());
    if (lobby != nullptr) {
        lobby->removeClient(client);
    }

    auto response = TCPData(DATATYPE_RESPONSE);
    response.add(RESPONSE, LEAVE_LOBBY);
    response.add(LEAVE_LOBBY, TRUE);

    sendMessage(client->getClientSocket(), response.serialize());
}

void MessageHandler::restoreClientState(const shared_ptr<Client>& client) {
    auto message = TCPData(DATATYPE_RESPONSE);
    message.add(RESPONSE, LOGIN);

    auto lobby = server.getLobby(client->getLobbyId());
    if (lobby == nullptr || !lobby->contains(client)) {
        message.add(RESTORE_STATE, LOBBY_LIST);
        sendMessage(client->getClientSocket(), message.serialize());
        sendLobbyList(client);
    } else {
        lobby->restoreState(client);
    }
}

void MessageHandler::sendClientReconnectedFromAnotherLocation(int clientSocket) {
    auto message = TCPData(DATATYPE_REQUEST);
    message.add(REQUEST, CONNECTION_CLOSED);
    sendMessage(clientSocket, message.serialize());
}

void MessageHandler::sendGameFinished(const shared_ptr<Client>& client) {
    auto message = TCPData(DATATYPE_REQUEST);
    message.add(REQUEST, REMOVED_FROM_LOBBY);
    sendMessage(client->getClientSocket(), message.serialize());
}

