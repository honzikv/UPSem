//
// Created by itznukeey on 20.11.19.
//

#include "Lobby.h"
#include "../communication/handlers/LobbyMessageHandler.h"
#include "controller/GameController.h"

void Lobby::addClient(shared_ptr<Client> client) {
    /*
     * Prida klienta do lobby a nastavi mu lobbyId a ze je v lobby - aby se pri nahlem odpojeni vratil do lobby pokud se
     * hra odstartovala
     */
    clients.push_back(client);
    client->setLobbyId(id);

    //Posle ostatnim klientum, ze se pripojil novy klient
    for (const auto& otherClient : clients) {
        if (client == otherClient) {
            continue;
        }
        lobbyMessageHandler->sendShowPlayerConnectedRequest(otherClient, client->getUsername());
    }
    //Posle vsem krome nove pripojeneho klienta novy seznam pripojenych - pripojeny klient seznam dostane v jine zprave
    lobbyMessageHandler->sendUpdatePlayerListRequest();
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
    }

    lobbyMessageHandler->sendUpdatePlayerListRequest(client);
    client->setLobbyId(-1);
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
            //Lobby zjisti, zda-li muze pripravit zacatek hry - tzn. zda-li vsichni klienti hlasovali
            if (canPrepareGameStart()) {
                /*
                Pokud lze, rekne controlleru aby odeslal zpravy pro pripravu hry - vsem hracum odesle dotaz kolik chteji
                vsadit
                */
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
            if (gameController->isGameFinished()) {
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
        resetClientParticipation();
        gameController->removeData();
    }
}

LobbyState Lobby::getLobbyState() const {
    return lobbyState;
}

void Lobby::sendClientDisconnected(const shared_ptr<Client>& client) {
    if (!contains(client)) {
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
    lobbyMessageHandler->sendUpdatePlayerListRequest();
}

void Lobby::sendClientDidntConfirm(const shared_ptr<Client>& client) {
    lobbyMessageHandler->sendClientDidntConfirm(client);
}

void Lobby::restoreState(const shared_ptr<Client>& client) {
    if (lobbyState == LOBBY_STATE_PREPARING) {
        auto request = TCPData(DATATYPE_REQUEST);
        request.add(RESPONSE, LOGIN);
        request.add(RESTORE_STATE, LOBBY);
        lobbyMessageHandler->sendMessage(client->getClientSocket(), request.serialize());
        lobbyMessageHandler->sendClientPlayerList(client);
    } else if (lobbyState == LOBBY_STATE_IN_GAME) {
        gameController->reconnectClient(client);
    }
}

bool Lobby::hasClientConfirmedToPlay(const shared_ptr<Client>& client) {
    return contains(client) && gameController->containsConfirmedClient(client);
}
