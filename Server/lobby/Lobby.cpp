//
// Created by itznukeey on 20.11.19.
//

#include "Lobby.h"
#include "LobbyMessageHandler.h"

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

    //Pokud je limit hracu naplnen, nastavi lobby jako not joinable
    if (clients.size() == limit) {
        joinable = false;
    }

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
}

bool Lobby::isJoinable() const {
    return joinable;
}

string Lobby::toString() {
    return to_string(id) + ";" + to_string(getClientCount()) + ";" + to_string(limit);
}

int Lobby::getClientCount() {
    return clients.size();
}

void Lobby::incrementVotes() {
    Lobby::votedToStart += 1;
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
    unprocessedMessages.emplace_back(make_shared<ClientData>(message, client));
}

const vector<shared_ptr<Client>>& Lobby::getClients() const {
    return clients;
}

void Lobby::removeClient(const shared_ptr<Client>& client) {
    clients.erase(remove(clients.begin(), clients.end(), client), clients.end());

    if (clients.size() < limit && lobbyState == LOBBY_STATE_WAITING) {
        joinable = true;
    }

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

    cout << "Client " << client->getUsername() << " left lobby " << id << endl;
}

void Lobby::handleLobby() {
    for (const auto& message : unprocessedMessages) {
        lobbyMessageHandler->handleMessage(message->getClient(), message->getMessage());
    }


    switch (lobbyState) {
        default:
        case LOBBY_STATE_WAITING:
            if (isPlayable()) {
                initializeGamePrep();
            }
            break;

        case LOBBY_STATE_PREPARING:
            if (prepTimeOut()) {
                startGame(gamePreparation->isPlayable());
            }
            break;

        case LOBBY_STATE_IN_PROGRESS:
            if (!blackjack->isGameRunning()) {
                endGame();
            }
            handleGameState();
            break;

        case LOBBY_STATE_FINISHED:
            checkIfReturnToLobby();
            break;
    }

}


bool Lobby::isPlayable() {
    if (lobbyState == LOBBY_STATE_IN_PROGRESS) {
        return false;
    }

    if (clients.size() == 2 && votedToStart == 2) {
        return true;
    }

    double voteStartRatio = votedToStart / (double) clients.size();
    return clients.size() > 2 && voteStartRatio > .66;
}

void Lobby::initializeGamePrep() {
    lobbyState = LOBBY_STATE_PREPARING;
    joinable = false;

    for (auto const& client : clients) {
        lobbyMessageHandler->sendPrepareGameSceneRequest(client);
    }
    gamePreparation = make_unique<GamePreparation>(clients.size());
}

bool Lobby::prepTimeOut() {
    if (gamePreparation == nullptr) {
        return true;
    }

    auto currentTime = chrono::system_clock::now();
    auto durationMillis = chrono::duration<double, milli>(
            currentTime - gamePreparation->getInitializationTime()).count();
    return durationMillis >= LOBBY_PREP_TIME_MS;
}

void Lobby::startGame(bool playable) {

    if (!playable) {
        cout << "Lobby " << id << " could not start, not enough active players " << endl;

        //Realisticky bude hrac pouze jeden
        for (const auto& client : gamePreparation->getConfirmedClients()) {
            lobbyMessageHandler->sendGameStartFailed(client);
        }

        votedToStart = 0;
        for (const auto& client : clients) {
            client->setHasVoted(false);
        }

        lobbyState = LOBBY_STATE_WAITING;
        joinable = clients.size() < limit;
        return;
    }

    joinable = false;
    lobbyState = LOBBY_STATE_IN_PROGRESS;

    auto confirmedClients = gamePreparation->getConfirmedClients();
    for (const auto& client : clients) {
        if (!(find(confirmedClients.begin(), confirmedClients.end(), client) != clients.end())) {
            lobbyMessageHandler->sendClientDidntConfirm(client);
            removeClient(client);
        }
    }

    blackjack = make_unique<Blackjack>(confirmedClients);
    blackjack->dealCards();

    sendBoardUpdate();
    lobbyMessageHandler->sendPlayersTurnRequest(blackjack->getCurrentPlayer());
}

void Lobby::confirmClient(shared_ptr<Client> client) {
    gamePreparation->addConfirmedClient(client);
}

void Lobby::handlePlayerTurn(const shared_ptr<Client>& client, const shared_ptr<TCPData>& message) {
    auto response = message->valueOf(TURN_TYPE);

    Result result;
    if (response == STAND) {
        result = blackjack->handleStand(client);
    } else {
        result = blackjack->handleHit(client);
    }

    if (result != RESULT_NOT_YOUR_TURN) {
        blackjack->moveToNextPlayer();
        lobbyMessageHandler->sendPlayersTurnRequest(blackjack->getCurrentPlayer());
        sendPlayerTurn();
        sendBoardUpdate();
    } else {
        lobbyMessageHandler->sendNotYourTurn(client);
    }

}

void Lobby::handleGameState() {
    //Pokud uzivatel nereaguje a nezvolil nic, hra za nej zvoli stand
    auto currentTime = chrono::system_clock::now();
    auto durationMillis = chrono::duration<double, milli>(
            currentTime - this->blackjack->getLastMessageSent()).count();
    if (durationMillis > MAX_TIME_BEFORE_STAND_MS) {
        blackjack->skipPlayer();
        blackjack->moveToNextPlayer();
        sendBoardUpdate();
    }
}

void Lobby::sendBoardUpdate() {
    for (auto const& player : blackjack->getPlayers()) {
        lobbyMessageHandler->sendBoard(player, blackjack->getPlayers(), blackjack->getDealer());
    }
}

void Lobby::endGame() {
    sendBoardUpdate();
    lobbyMessageHandler->sendResults(blackjack->getPlayers(), blackjack->getDealer());
    gamePreparation = nullptr;
    blackjack = nullptr;

    for (const auto& client : clients) {
        client->setHasVoted(false);
        votedToStart = 0;
    }

    returnToLobbyStart = chrono::system_clock::now();
}

void Lobby::checkIfReturnToLobby() {
    auto currentTime = chrono::system_clock::now();
    auto durationMillis = chrono::duration<double, milli>(currentTime - returnToLobbyStart).count();
    if (durationMillis >= TIME_BEFORE_RETURN_TO_LOBBY) {
        lobbyState = LOBBY_STATE_WAITING;
        joinable = clients.size() < limit;

        for (const auto& client : clients) {
            lobbyMessageHandler->sendShowLobbyRequest(client);
        }
    }
}
