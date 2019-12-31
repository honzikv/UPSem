//
// Created by itznu on 12/25/2019.
//

#include "LobbyMessageHandler.h"
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
        if (response == CONFIRM_PARTICIPATION && lobby.getLobbyState() == LOBBY_STATE_PREPARING) {
            //pridame klienta do seznamu potvrzenych ucastniku hry
            lobby.confirmClient(client);
        } else if (response == UPDATE_BOARD) {
            //todo confirm
        } else if (response == TURN) {
            lobby.handlePlayerTurn(client, message);
        }
    }
    catch (exception&) {
        //todo kick client
    }
}

void LobbyMessageHandler::handleRequest(const shared_ptr<Client>& client, const shared_ptr<TCPData>& message) {
    try {
        auto request = message->valueOf(REQUEST);

        if (request == SEND_READY) {
            if (!client->isReady()) {
                client->setReady(true);
                for (const auto& lobbyClient : lobby.getClients()) {
                    sendUpdatePlayerListRequest(client);
                }
            }
            lobby.incrementVotes();
            cout << "incremented votes" << endl;
        }
    }
    catch (exception&) {
        cout << "asdaasdasdas" << endl;
    }

}

LobbyMessageHandler::LobbyMessageHandler(Lobby& lobby) : lobby(lobby) {
}

void LobbyMessageHandler::sendShowPlayerDisconnectedRequest(const shared_ptr<Client>& client,
                                                            const string& disconnectedPlayer) {
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

void
LobbyMessageHandler::sendShowPlayerConnectedRequest(const shared_ptr<Client>& client, const string& connectedPlayer) {
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
        message.add(CLIENT + to_string(clientNo), currentClient->toString());
        clientNo++;
    }

    sendMessage(client->getClientSocket(), message.serialize());
}

void LobbyMessageHandler::sendPrepareGameSceneRequest(const shared_ptr<Client>& client) {
    auto message = TCPData(DATATYPE_REQUEST);
    message.add(REQUEST, CONFIRM_PARTICIPATION);
    sendMessage(client->getClientSocket(), message.serialize());
}

void LobbyMessageHandler::sendGameStartFailed(const shared_ptr<Client>& client) {
    auto message = TCPData(DATATYPE_REQUEST);
    message.add(REQUEST, SHOW_GAME_START_FAILED);
    sendMessage(client->getClientSocket(), message.serialize());
}

void LobbyMessageHandler::sendBoard(const shared_ptr<Client>& client, const vector<shared_ptr<Client>>& players,
                                    const shared_ptr<Dealer>& dealer) {
    auto message = TCPData(DATATYPE_REQUEST);
    message.add(REQUEST, UPDATE_BOARD);

    auto playerNo = 0;
    for (const auto& player : players) {
        message.add(PLAYER + to_string(playerNo), player->getUsername());
        message.add(PLAYER + to_string(playerNo) + TOTAL_VALUE, to_string(player->getPlayerInfo()->getHandValue()));
        auto cardNo = 0;
        for (const auto& card : player->getPlayerInfo()->getHand()) {
            message.add(PLAYER + to_string(playerNo) + CARD + to_string(cardNo), card->toString());
            cardNo++;
        }
        playerNo++;
    }
    message.add(PLAYER_COUNT, to_string(players.size()));
    auto cardNo = 0;
    for (const auto& card : dealer->getPlayerInfo()->getHand()) {
        message.add(DEALER + string(CARD) + to_string(cardNo), card->toString());
        cardNo++;
    }

    sendMessage(client->getClientSocket(), message.serialize());
}

void LobbyMessageHandler::sendResults(const vector<shared_ptr<Client>>& clients, const shared_ptr<Dealer>& dealer) {
    auto message = TCPData(DATATYPE_REQUEST);
    message.add(REQUEST, SHOW_RESULTS);
    message.add(TOTAL_VALUE, to_string(dealer->getPlayerInfo()->getHandValue()));

    auto dealerInfo = dealer->getPlayerInfo();
    auto playerNo = 0;
    for (const auto& player : clients) {
        auto clientInfo = player->getPlayerInfo();
        if (dealerInfo->isBusted() && !clientInfo->isBusted()) {
            message.add(PLAYER + to_string(playerNo), WIN);
        } else if (dealerInfo->isBusted() && clientInfo->isBusted()) {
            message.add(PLAYER + to_string(playerNo), LOSS);
        } else {
            message.add(PLAYER + to_string(playerNo),
                        dealerInfo->getHandValue() > clientInfo->getHandValue() ? LOSS : WIN);
        }

        playerNo++;
    }

    auto serializedMessage = message.serialize();
    for (const auto& player : clients) {
        sendMessage(player->getClientSocket(), serializedMessage);
    }
}

void LobbyMessageHandler::sendPlayersTurnRequest(const shared_ptr<Client>& client) {
    if (client == nullptr) {
        return;
    }

    auto message = TCPData(DATATYPE_REQUEST);
    message.add(REQUEST, TURN);
    sendMessage(client->getClientSocket(), message.serialize());
}

void LobbyMessageHandler::sendNotYourTurn(const shared_ptr<Client>& client) {
    auto message = TCPData(DATATYPE_REQUEST);
    message.add(REQUEST, NOT_YOUR_TURN);
    sendMessage(client->getClientSocket(), message.serialize());
}

void LobbyMessageHandler::sendClientDidntConfirm(const shared_ptr<Client>& client) {
    auto message = TCPData(DATATYPE_REQUEST);
    message.add(REQUEST, CLIENT_DIDNT_CONFIRM);
    sendMessage(client->getClientSocket(), message.serialize());
}

void LobbyMessageHandler::sendShowLobbyRequest(const shared_ptr<Client>& client) {
    auto message = TCPData(DATATYPE_REQUEST);
    message.add(REQUEST, JOIN_LOBBY);
    message.add(LOBBY_ID, to_string(lobby.getId()));

    auto clientNo = 0;
    for (const auto& currentClient : lobby.getClients()) {
        message.add(CLIENT + to_string(clientNo), currentClient->getUsername());
        clientNo++;
    }

    sendMessage(client->getClientSocket(), message.serialize());
}

void LobbyMessageHandler::sendPlayerTurn(const shared_ptr<TurnResult>& turnResult, const shared_ptr<Client>& client) {
    auto message = TCPData(DATATYPE_REQUEST);
    message.add(REQUEST, SHOW_PLAYER_TURN);
    message.add(USERNAME, turnResult->getClient()->getUsername());
    if (turnResult->getCard() == nullptr) {
        message.add(TURN_TYPE, STAND);
    } else {
        message.add(TURN_TYPE, HIT);
        message.add(CARD, turnResult->getCard()->toString());
    }

    sendMessage(client->getClientSocket(), message.serialize());
}


