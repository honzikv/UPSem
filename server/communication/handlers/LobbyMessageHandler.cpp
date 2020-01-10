//
// Created by itznu on 12/25/2019.
//

#include "LobbyMessageHandler.h"
#include "../../lobby/controller/GameController.h"

LobbyMessageHandler::LobbyMessageHandler(Lobby& lobby) : lobby(lobby) {
}

void LobbyMessageHandler::sendMessage(int socket, const string& message) {
    send(socket, message.c_str(), message.size(), 0);
}

void LobbyMessageHandler::handleMessage(const shared_ptr<Client>& client, const shared_ptr<TCPData>& message) {
    auto dataTypeFieldValue = message->valueOf(DATA_TYPE);

    if (dataTypeFieldValue == RESPONSE) {
        handleResponse(client, message);
    } else {
        handleRequest(client, message);
    }
}

void LobbyMessageHandler::handleResponse(const shared_ptr<Client>& client, const shared_ptr<TCPData>& message) {
    auto response = message->valueOf(RESPONSE);
    if (response == CONFIRM_PARTICIPATION && lobby.getLobbyState() == LOBBY_STATE_PREPARING) {
        lobby.getGameController()->addConfirmedClient(client, stoi(message->valueOf(BET)));
    } else if (response == TURN) {
        lobby.getGameController()->handlePlayerTurn(client, message);
    } else if (response == DECLINE_PARTICIPATION) {
        lobby.removeClient(client);
        this->sendDeclineAcknowledged(client);
    }

}

void LobbyMessageHandler::handleRequest(const shared_ptr<Client>& client, const shared_ptr<TCPData>& message) {
    auto request = message->valueOf(REQUEST);
    if (request == SEND_READY) {
        if (!client->isReady()) {
            client->setReady(true);
            sendUpdatePlayerListRequest();
            lobby.incrementPlayersReady();
        }
    }
}

void LobbyMessageHandler::sendShowPlayerDisconnectedRequest(const shared_ptr<Client>& client,
                                                            const string& disconnectedPlayer) {
    auto message = TCPData(DATATYPE_REQUEST);
    message.add(REQUEST, SHOW_PLAYER_DISCONNECTED);
    message.add(USERNAME, disconnectedPlayer);
    sendMessage(client->getClientSocket(), message.serialize());
}


void
LobbyMessageHandler::sendShowPlayerConnectedRequest(const shared_ptr<Client>& client, const string& connectedPlayer) {
    auto message = TCPData(DATATYPE_REQUEST);
    message.add(REQUEST, SHOW_PLAYER_CONNECTED);
    message.add(USERNAME, connectedPlayer);
    sendMessage(client->getClientSocket(), message.serialize());
}

void LobbyMessageHandler::sendUpdatePlayerListRequest() {
    auto serializedMessage = playerListSerializedString();
    for (const auto& client : lobby.getClients()) {
        sendMessage(client->getClientSocket(), serializedMessage);
    }
}

void LobbyMessageHandler::sendClientPlayerList(const shared_ptr<Client>& client) {
    sendMessage(client->getClientSocket(), playerListSerializedString());
}

string LobbyMessageHandler::playerListSerializedString() {
    auto message = TCPData(DATATYPE_REQUEST);
    message.add(REQUEST, UPDATE_PLAYER_LIST);

    auto clientNo = 0;
    for (const auto& client : lobby.getClients()) {
        message.add(CLIENT + to_string(clientNo), client->toString());
        clientNo++;
    }
    return message.serialize();
}

void LobbyMessageHandler::sendUpdatePlayerListRequest(const shared_ptr<Client>& exclude) {
    auto serializedMessage = playerListSerializedString();
    for (const auto& currentClient : lobby.getClients()) {
        if (exclude == currentClient) {
            continue;
        }
        sendMessage(currentClient->getClientSocket(), serializedMessage);
    }
}

void LobbyMessageHandler::sendClientDidntConfirm(const shared_ptr<Client>& client) {
    auto message = TCPData(DATATYPE_REQUEST);
    message.add(REQUEST, CLIENT_DIDNT_CONFIRM);
    sendMessage(client->getClientSocket(), message.serialize());
}

void LobbyMessageHandler::sendShowLobbyRequest(const shared_ptr<Client>& client) {
    auto message = TCPData(DATATYPE_REQUEST);
    message.add(REQUEST, JOIN_LOBBY);
    sendMessage(client->getClientSocket(), message.serialize());
}

void LobbyMessageHandler::sendDeclineAcknowledged(const shared_ptr<Client>& client) {
    auto message = TCPData(DATATYPE_REQUEST);
    message.add(REQUEST, REMOVED_FROM_LOBBY);
    sendMessage(client->getClientSocket(), message.serialize());
}
