//
// Created by itznukeey on 20.11.19.
//

#include "Lobby.h"
#include "LobbyMessageHandler.h"

bool Lobby::addClient(const shared_ptr<Client>& client) {
    if (find(clients.begin(), clients.end(), client) != clients.end()) {
        return false;
    }

    clients.push_back(client);
    client->setLobbyId(id);
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
    lastPlayerListUpdate = chrono::system_clock::now();

    return true;
}


Lobby::Lobby(int limit, int id) : limit(limit), id(id) {
    this->lobbyMessageHandler = make_shared<LobbyMessageHandler>(*this);
}

bool Lobby::isJoinable() const {
    return joinable;
}

string Lobby::getState() {
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

    if (clients.size() < limit) {
        joinable = true;
    }

    //Posle ostatnim klientum, ze se pripojil novy klient
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

void Lobby::processMessages() {
    for (const auto& message : unprocessedMessages) {
        lobbyMessageHandler->handleMessage(message->getClient(), message->getMessage());
    }
}

bool Lobby::isTimeToPlay() {
    if (lobbyState == LOBBY_STATE_RUNNING) {
        return false;
    }

    if (clients.size() == 2 && votedToStart == 2) {
        return true;
    }

    double voteStartRatio = votedToStart / (double) clients.size();
    return clients.size() > 2 && voteStartRatio > .66;
}

void Lobby::updatePlayerResponseTime(const string& username) {
    auto client = clientResponseTimes.find(username);

    if (client != clientResponseTimes.end()) {
        client->second = chrono::system_clock::now();
    }
}

void Lobby::startGame() {

}

void Lobby::handleGameState() {

}
