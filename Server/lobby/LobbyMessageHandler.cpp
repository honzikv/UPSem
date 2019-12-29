//
// Created by itznu on 12/25/2019.
//

#include "LobbyMessageHandler.h"
#include "Lobby.h"
#include "../communication/Fields.h"
#include "../communication/Values.h"

void LobbyMessageHandler::handleMessage(const shared_ptr<Client>& client, const shared_ptr<TCPData>& message) {

    try {
        auto dataTypeFieldValue = message->valueOf(DATA_TYPE);

        if (dataTypeFieldValue == RESPONSE) {
            handleResponse(client, message);
        } else {
            handleRequest(client, message);
        }
    }
    catch (exception&) {
        //todo kick client
        cout << "sdgsdgs" << endl;
    }

}

void LobbyMessageHandler::handleResponse(const shared_ptr<Client>& client, const shared_ptr<TCPData>& message) {
    try {
        auto response = message->valueOf(RESPONSE);

        lobby.updatePlayerResponseTime(client->getUsername());
    }
    catch (exception&) {
        //todo kick client
    }
}

void LobbyMessageHandler::handleRequest(const shared_ptr<Client>& client, const shared_ptr<TCPData>& message) {
    try {
        auto request = message->valueOf(REQUEST);

        if (request == message->valueOf(VOTE_START)) {
            if (!client->hasVoted()) {
                client->setHasVoted(true);
            }
            lobby.incrementVotes();
        }
    }
    catch (exception&) {}

}

LobbyMessageHandler::LobbyMessageHandler(Lobby& lobby) : lobby(lobby) {
}

void LobbyMessageHandler::sendShowPlayerDisconnectedRequest(const shared_ptr<Client>& client, const string& disconnectedPlayer) {
    auto message = TCPData(DATATYPE_REQUEST);
    message.add(REQUEST, SHOW_PLAYER_DISCONNECTED);
    message.add(USERNAME, disconnectedPlayer);
    sendMessage(client->getClientSocket(), message.serialize());
}

void LobbyMessageHandler::sendMessage(int socket, const string& message) {
    cout << message;
    //todo exception handling
    send(socket, message.c_str(), message.size(), 0);
}

void LobbyMessageHandler::sendShowPlayerConnectedRequest(const shared_ptr<Client>& client, const string& connectedPlayer) {
    auto message = TCPData(DATATYPE_REQUEST);
    message.add(REQUEST, SHOW_PLAYER_CONNECTED);
    message.add(USERNAME, connectedPlayer);
    sendMessage(client->getClientSocket(), message.serialize());
}

void LobbyMessageHandler::sendUpdatePlayerListRequest(const shared_ptr<Client>& client) {
    auto message = TCPData(DATATYPE_REQUEST);
    message.add(REQUEST, UPDATE_PLAYER_LIST);

    auto clientNo = 0;
    for (const auto& currentClient : lobby.getClients()) {
        message.add(CLIENT + to_string(clientNo), currentClient->getUsername());
        clientNo++;
    }

    sendMessage(client->getClientSocket(), message.serialize());
}

