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
    if (lobbyState == LOBBY_STATE_IN_GAME) {
        gameController->addAutoSkip(client);
    }
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

    cout << "Client " << client->getUsername() << " was removed from lobby " << id << endl;
}

void Lobby::addDisconnectedClient(const shared_ptr<Client>& client) {
    disconnectedClients.push_back(client);
}

const shared_ptr<GameController>& Lobby::getGameController() const {
    return gameController;
}

void Lobby::handleLobby() {
    //Zpracovani vsech nezpracovanych zprav
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
            //Pokud vyprsel cas kdy mohli vsichni vlozit sazky, zkontroluje se jestli je hra hratelna a pokud ano odstartuje se
            if (gameController->hasPreparationTimeExpired()) {
                if (gameController->isPlayable()) {
                    gameController->startGame();
                } else {
                    gameController->cancelGame();
                }
            }
            break;

        case LOBBY_STATE_IN_GAME:
            /*
             * Pokud je hra dokoncena, GameController posle vsem vysledky, jinak se kontroluje,
             * jestli je cas na dalsiho hrace - odpovedi prichazeji asynchronne - tzn. odpoved musi prijit do daneho
             * casoveho limitu
             */
            if (gameController->isGameFinished()) {
                gameController->endGame();
            } else {
                gameController->checkIfTimeForNextPlayer();
            }
            break;

        case LOBBY_STATE_FINISHED:
            //Kontrola jestli vyprsel cas pro navrat do lobby
            checkIfReturnToLobby();
            break;
    }
}

void Lobby::checkIfReturnToLobby() {
    auto currentTime = system_clock::now();
    auto durationMillis = duration<double, milli>(currentTime - returnToLobbyStart).count();

    //Pokud nastal cas pro navrat do lobby
    if (durationMillis >= TIME_BEFORE_RETURN_TO_LOBBY || clients.empty()) {
        lobbyState = LOBBY_STATE_WAITING;

        //Vsem klientum odesleme pozadavek na zobrazeni lobby, resetujeme jejich ucast a odstranime data o hre
        for (const auto& client : clients) {
            lobbyMessageHandler->sendShowLobbyRequest(client);
        }
        lobbyMessageHandler->sendUpdatePlayerListRequest();
        resetClientParticipation();
        gameController->removeData();

        //Vsichni klienti co se v dobe hry odpojili budou odstraneni
        for (const auto& client : disconnectedClients) {
            removeClient(client);
        }
        disconnectedClients.clear();
    }
}

LobbyState Lobby::getLobbyState() const {
    return lobbyState;
}

void Lobby::sendClientDisconnected(const shared_ptr<Client>& client) {
    if (!contains(client)) {
        return;
    }
    for (const auto& player : clients) {
        if (player == client) {
            continue;
        }
        lobbyMessageHandler->sendShowPlayerDisconnectedRequest(player, client->getUsername());
    }
}

void Lobby::setLobbyState(LobbyState lobbyState) {
    Lobby::lobbyState = lobbyState;
}

void Lobby::prepareToReturn() {
    returnToLobbyStart = system_clock::now();
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
    if (lobbyState == LOBBY_STATE_PREPARING || lobbyState == LOBBY_STATE_WAITING) {
        auto request = TCPData(DATATYPE_RESPONSE);
        request.add(RESPONSE, LOGIN);
        request.add(RESTORE_STATE, LOBBY);
        lobbyMessageHandler->sendMessage(client->getClientSocket(), request.serialize());
        lobbyMessageHandler->sendClientPlayerList(client);
    } else if (lobbyState == LOBBY_STATE_IN_GAME || lobbyState == LOBBY_STATE_FINISHED) {
        gameController->reconnectClient(client);
        disconnectedClients.erase(remove(disconnectedClients.begin(), disconnectedClients.end(), client),
                                  disconnectedClients.end());
    }
}