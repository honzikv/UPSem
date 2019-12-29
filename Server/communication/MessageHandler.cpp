//
// Created by nvk on 12/26/2019.
//

#include "MessageHandler.h"
#include "Values.h"
#include "Fields.h"
#include "../Server.h"

MessageHandler::MessageHandler(Server& server) : server(server) {}

void MessageHandler::sendMessage(int socket, const string& message) {
    cout << message;
    if (send(socket, message.c_str(), message.size(), 0) <= 0) {
        server.closeConnection(socket);
    }
}

void MessageHandler::handleMessage(int clientSocket, const shared_ptr<TCPData>& message) {

    try {
        auto dataType = message->valueOf(DATATYPE);
        if (dataType == REQUEST) {
            handleRequest(clientSocket, message);
        } else if (dataType == RESPONSE) {
            handleResponse(clientSocket, message);
        } else if (dataType == PING) {
            pingBack(clientSocket);
        }
    }
        //Pokud zachyti chybu, hodi ji do select funkce, kde se o ni postara server
    catch (exception& ex) {
        cerr << ex.what();
        throw DeserializationException();
    }

}

void MessageHandler::sendLobbyList(const shared_ptr<Client>& client) {
    auto message = TCPData(DATATYPE_RESPONSE);
    message.add(RESPONSE, LOBBY_LIST);
    for (const auto& lobby : server.getLobbies()) {
        message.add("lobby" + to_string(lobby->getId()), lobby->getState());
    }

    sendMessage(client->getClientSocket(), message.serialize());
}


void MessageHandler::pingBack(int clientSocket) {
    try {
        auto message = TCPData(DATATYPE_RESPONSE);
        message.add(RESPONSE, PING);
        sendMessage(clientSocket, message.serialize());
    }
    catch (exception& exception) {
        throw exception;
    }
}

void MessageHandler::handleRequest(int clientSocket, const shared_ptr<TCPData>& message) {
    auto request = message->valueOf(REQUEST);

    try {
        if (request == LOGIN) {
            auto username = message->valueOf(USERNAME);
            auto client = server.getClientByUsername(username);
            if (client == nullptr) {
                server.registerClient(username, clientSocket);
                sendLoginIsNew(clientSocket);
                cout << "Welcome \"" << username << "\" on the server " << endl;
            } else {
                client = server.getClientByUsername(username);
                server.closeConnection(client->getClientSocket());
                //todo send relog from another location
                client->setClientSocket(clientSocket);
                sendClientReconnected(clientSocket);
                client->updateLastMessageReceived();
                cout << "Client \"" << username << "\" was reconnected " << endl;
            }
        } else {
            auto client = server.getClientBySocket(clientSocket);

            if (request == LOBBY_LIST) {
                //posle lobby list
                sendLobbyList(client);
            } else if (request == JOIN_LOBBY) {
                //posle info o lobby, pokud je lobby plna posle pouze ze je plna, jinak posle i jmena hracu
                sendLobby(client, message);
            } else if (request == LEAVE_LOBBY) {
                leaveLobby(client, message);
            } else if (client->isInLobby()) {
                auto lobby = server.getLobby(client->getLobbyId());
                lobby->addNewMessage(message, client);
            }

            client->updateLastMessageReceived();
        }
    }
    catch (exception& exception) {
        cout << "exception " << endl;
        throw exception;
    }
}

void MessageHandler::sendLoginIsNew(int clientSocket) {
    try {
        auto message = TCPData(DATATYPE_RESPONSE);
        message.add(RESPONSE, LOGIN);
        message.add(IS_NEW, TRUE);
        sendMessage(clientSocket, message.serialize());
    }
    catch (exception& exception) {
        throw exception;
    }
}

void MessageHandler::handleResponse(int clientSocket, const shared_ptr<TCPData>& message) {
    auto response = message->valueOf(RESPONSE);
    auto client = server.getClientBySocket(clientSocket);

//    if (client == nullptr) {
//        cout << "Closing unauthenticated socket" << endl;
//        server.closeConnection(clientSocket);
//        return;
//    }
//
//    auto lobby = server.getLobby(client->getLobbyId());
//
//    if (lobby == nullptr) {
//        cout << "Closing incorrect response" << endl;
//        server.removeClient(client);
//    }
//    else {
//        lobby->addNewMessage(message, client);
//    }

}

void MessageHandler::sendLobby(shared_ptr<Client>& client, const shared_ptr<TCPData>& message) {
    try {
        auto response = TCPData(DATATYPE_RESPONSE);
        response.add(RESPONSE, JOIN_LOBBY);
        auto lobbyId = stoi(message->valueOf(LOBBY_ID));
        auto isJoinable = server.isLobbyJoinable(lobbyId);
        response.add(IS_JOINABLE, isJoinable ? TRUE : FALSE);

        if (isJoinable) {
            response.add(LOBBY_ID, to_string(lobbyId));
            auto lobby = server.getLobby(lobbyId);
            lobby->addClient(client);
            auto clientNo = 0;
            for (const auto& currentClient : lobby->getClients()) {
                response.add(CLIENT + to_string(clientNo), currentClient->getUsername());
                clientNo++;
            }
        }

        sendMessage(client->getClientSocket(), response.serialize());
    }
    catch (exception& exception) {
        throw exception;
    }
}

void MessageHandler::leaveLobby(shared_ptr<Client>& client, const shared_ptr<TCPData>& message) {
    try {
        auto lobby = server.getLobby(client);
        if (lobby != nullptr) {
            lobby->removeClient(client);
        }

        auto response = TCPData(DATATYPE_RESPONSE);
        response.add(RESPONSE, LEAVE_LOBBY);
        response.add(LEAVE_LOBBY, TRUE);

        sendMessage(client->getClientSocket(), response.serialize());
    }
    catch (exception& exception) {
        throw exception;
    }
}

void MessageHandler::sendClientReconnected(int clientSocket) {
    try {
        auto message = TCPData(DATATYPE_RESPONSE);
        message.add(RESPONSE, LOGIN);
        message.add(IS_NEW, FALSE);
    }
    catch (exception& exception) {
        throw exception;
    }
}
