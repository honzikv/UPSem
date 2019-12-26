//
// Created by itznu on 12/25/2019.
//

#include "LobbyMessageHandler.h"
#include "Lobby.h"
#include "../communication/Fields.h"
#include "../communication/Values.h"

void LobbyMessageHandler::handleMessage(shared_ptr<Client>& client, shared_ptr<TCPData>& message) {

    try {
        auto dataTypeFieldValue = message->valueOf(DATA_TYPE);
        if (dataTypeFieldValue == PING) {
            //todo ping back
        }

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

void LobbyMessageHandler::handleResponse(shared_ptr<Client>& client, shared_ptr<TCPData>& message) {
    try {
        auto response = message->valueOf(RESPONSE);

    }
    catch (exception&) {
        //todo kick client
    }
}

void LobbyMessageHandler::handleRequest(shared_ptr<Client>& client, shared_ptr<TCPData>& message) {
    try {
        auto request = message->valueOf(REQUEST);

        if (request == message->valueOf(VOTE_START)) {
            if (!client->hasVoted()) {
                client->setHasVoted(true);
            }
        }
    }
    catch (exception&) {}

}

LobbyMessageHandler::LobbyMessageHandler(Lobby& lobby) : lobby(lobby) {
}


