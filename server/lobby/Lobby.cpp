//
// Created by itznukeey on 20.11.19.
//

#include "Lobby.h"
#include "../communication/handlers/LobbyMessageHandler.h"
#include "controller/GameController.h"

bool Lobby::addClient(shared_ptr<Client> client) {
    //Teoreticky se nikdy nestane, pouze pro jistotu, ze vector obsahuje unikatni prvky
    if (find(clients.begin(), clients.end(), client) != clients.end()) {
        return false;
    }

    /*
     * Prida klienta do lobby a nastavi mu lobbyId a ze je v lobby - aby se pri nahlem odpojeni vratil do lobby pokud se
     * hra odstartovala
     */
    clients.push_back(client);
    client->setLobbyId(id);
    client->setInLobby(true);

    //Posle ostatnim klientum, ze se pripojil novy klient
    for (const auto& otherClient : clients) {
        if (client == otherClient) {
            continue;
        }
        lobbyMessageHandler->sendShowPlayerConnectedRequest(otherClient, client->getUsername());
        lobbyMessageHandler->sendUpdatePlayerListRequest(otherClient);
    }

    return true;
}

Lobby::Lobby(int limit, int id) : limit(limit), id(id) {
    this->lobbyMessageHandler = make_shared<LobbyMessageHandler>(*this);
    this->gameController = make_shared<GameController>(*this);
}

bool Lobby::isJoinable() const {
    return lobbyState == LOBBY_STATE_WAITING && clients.size() < limit;
}

string Lobby::toString() {
    return to_string(id) + ";" + to_string(getClientCount()) + ";" + to_string(limit);
}

int Lobby::getClientCount() {
    return clients.size();
}

void Lobby::incrementPlayersReady() {
    Lobby::clientsReady += 1;
}

int Lobby::getId() const {
    return id;
}

bool Lobby::contains(const shared_ptr<Client>& client) {
    //projde seznam klientu, pokud shared pointer z parametru je shodny s nejakym shared pointerem ze seznamu vrati true
    for (const auto& lobbyClient : clients) {
        if (lobbyClient->getUsername() == client->getUsername()) {
            return true;
        }
    }

    return false;
}

void Lobby::addNewMessage(const shared_ptr<TCPData>& message, const shared_ptr<Client>& client) {
    unprocessedMessages.emplace(make_shared<ClientData>(message, client));
}

const vector<shared_ptr<Client>>& Lobby::getClients() const {
    return clients;
}

void Lobby::removeClient(const shared_ptr<Client>& client) {
    clients.erase(remove(clients.begin(), clients.end(), client), clients.end());

    //Posle ostatnim klientum, ze se klient odpojil
    for (const auto& otherClient : clients) {
        if (client == otherClient) {
            continue;
        }
        lobbyMessageHandler->sendShowPlayerDisconnectedRequest(otherClient, client->getUsername());
        lobbyMessageHandler->sendUpdatePlayerListRequest(otherClient);
    }

    client->setLobbyId(-1);
    client->setInLobby(false);
    client->setReady(false);

    cout << "Client " << client->getUsername() << " left lobby " << id << endl;
}

const shared_ptr<GameController>& Lobby::getGameController() const {
    return gameController;
}

void Lobby::handleLobby() {
    while (!unprocessedMessages.empty()) {
        auto message = unprocessedMessages.front();
        unprocessedMessages.pop();
        lobbyMessageHandler->handleMessage(message->getClient(), message->getMessage());
    }

    switch (lobbyState) {
        default:
        case LOBBY_STATE_WAITING:
            if (canPrepareGameStart()) {
                gameController->prepareGame();
            }
            break;

        case LOBBY_STATE_PREPARING:
            if (gameController->hasPreparationTimeExpired()) {
                if (gameController->isPlayable()) {
                    gameController->startGame();
                } else {
                    gameController->cancelGame();
                }
            }
            break;

        case LOBBY_STATE_IN_GAME:
            if (gameController->hasGameFinished()) {
                gameController->endGame();
            }
            gameController->checkIfTimeForNextPlayer();
            break;

        case LOBBY_STATE_FINISHED:
            checkIfReturnToLobby();
            break;
    }
}

void Lobby::checkIfReturnToLobby() {
    auto currentTime = chrono::system_clock::now();
    auto durationMillis = chrono::duration<double, milli>(currentTime - returnToLobbyStart).count();
    if (durationMillis >= TIME_BEFORE_RETURN_TO_LOBBY) {
        lobbyState = LOBBY_STATE_WAITING;

        for (const auto& client : clients) {
            lobbyMessageHandler->sendShowLobbyRequest(client);
        }
    }
}

LobbyState Lobby::getLobbyState() const {
    return lobbyState;
}

bool Lobby::reconnectClient(const shared_ptr<Client>& client) {
    if (!this->contains(client)) {
        return false;
    }
    gameController->reconnectClient(client);
    return true;
}

void Lobby::sendClientDisconnected(const shared_ptr<Client>& client) {
    if (!this->contains(client)) {
        return;
    }
    auto message = TCPData(DATATYPE_REQUEST);
    message.add(REQUEST, SHOW_PLAYER_DISCONNECTED);
    message.add(USERNAME, "Player " + client->getUsername() + " has disconnected");

    for (const auto& player : clients) {
        if (player == client) {
            continue;
        }
        lobbyMessageHandler->sendMessage(player->getClientSocket(), message.serialize());
    }
}

void Lobby::sendClientReconnected(const shared_ptr<Client>& client) {
    auto message = TCPData(DATATYPE_REQUEST);
    message.add(REQUEST, SHOW_PLAYER_CONNECTED);
    message.add(USERNAME, "Player " + client->getUsername() + " has disconnected");
    for (const auto& player : clients) {
        lobbyMessageHandler->sendMessage(player->getClientSocket(), message.serialize());
    }
}

void Lobby::setLobbyState(LobbyState lobbyState) {
    Lobby::lobbyState = lobbyState;
}

void Lobby::prepareToReturn() {
    returnToLobbyStart = chrono::system_clock::now();
}

bool Lobby::canPrepareGameStart() {
    return clientsReady == clients.size() && clients.size() >= 2;
}

void Lobby::resetClientParticipation() {
    for (const auto& client : clients) {
        client->setReady(false);
    }
    clientsReady = 0;
}

void Lobby::sendClientDidntConfirm(const shared_ptr<Client>& client) {
    lobbyMessageHandler->sendClientDidntConfirm(client);
}
