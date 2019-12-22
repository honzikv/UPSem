//
// Created by itznukee on 11/20/19.
//

#include "Server.h"

Server::Server(int port, int lobbyCount) {

    //Nastaveni filedescriptoru socketu serveru
    if ((mainSocket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        cerr << "Error, OS could not create socket for the Server, try again" << endl;
        exit(EXIT_FAILURE);
    }


    //Nastaveni adresy serveru
    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_port = htons(port);

    //bind socketu
    if (bind(mainSocket, (struct sockaddr*) &address, sizeof(address)) < 0) {
        cerr << "Error, OS could not bind socket, try again" << endl;
        exit(EXIT_FAILURE);
    }
    cout << "Successfully bound socket to Server, ready to run." << endl;

    for (auto i = 0; i < lobbyCount; i++) {
        lobbies.push_back(make_shared<Lobby>(MAX_CLIENTS_PER_LOBBY, i));
    }
}

void Server::run() {

    cout << "Launching Server ..." << endl;
    int maxSocket;
    vector<int> sockets;
    fd_set socketSet;

    sockaddr_in clientInfo;

    //Vynulovani socket setu a nastaveni hlavniho socketu
    FD_ZERO(&socketSet);
    FD_SET(mainSocket, &socketSet);
    //Nastaveni maximalniho socketu jako socketu serveru
    maxSocket = mainSocket;

    //TODO nastavit timeout
    while (select(maxSocket + 1, &socketSet, NULL, NULL, 0) > 0) {
        if (FD_ISSET(mainSocket, &socketSet)) {
            socklen_t addressLength = sizeof(clientInfo);
            auto clientSocket = accept(mainSocket, (sockaddr*) &clientInfo, &addressLength);
            sockets.push_back(clientSocket);
        }

        for (auto socket : sockets) {
            if (FD_ISSET(socket, &socketSet)) {
                char buffer[MAX_BUFFER_SIZE_BYTES];

                if ((recv(socket, (void*) buffer, MAX_BUFFER_SIZE_BYTES - 1, 0)) <= 0) {
                    sockets.erase(remove(sockets.begin(), sockets.end(), socket), sockets.end());
                    close(socket);
                    break; //concurrentmodificationexception
                } else {
                    //handle
                    try {
                        auto message = TCPData(buffer);
                        auto client = getClient(socket);

                        if (client == nullptr) {
                            auto username = message.valueOf(USERNAME);
                            if (isLoginUnique(username)) {
                                sendLoginUnique(socket, true);
                                clients.push_back(make_shared<Client>(username, socket));
                            } else {
                                sendLoginUnique(socket, false);
                            }
                        } else {
                            auto requestValue = message.valueOf(REQUEST);

                            if (requestValue == LOBBY_LIST) {
                                sendLobbyList(client);
                            }

                            if (requestValue == JOIN_LOBBY) {
                                auto lobby = getLobby(stoi(message.valueOf(LOBBY_ID)));
                                if (lobby == nullptr) {
                                    throw exception();
                                }

                                if (lobby->isJoinable()) {
                                    lobby->addClient(client);
                                    sendLobbyJoinable(client.operator*(), true);
                                    //Klienta připojíme do lobby a odstraníme ho z aktivních socketů selectu
                                    sockets.erase(remove(sockets.begin(), sockets.end(), socket), sockets.end());
                                    break;
                                } else {
                                    sendLobbyJoinable(client.operator*(), false);
                                }
                            }
                        }

                    }
                    catch (exception&) {
                        cerr << "Error client provided incorrect input, disconnecting" << endl;
                        close(socket);
                    }
                }
            }
        }

        FD_ZERO(&socketSet);
        FD_SET(mainSocket, &socketSet);
        maxSocket = 0;

        for (auto socket : sockets) {
            if (socket > maxSocket) {
                maxSocket = socket;
            }

            FD_SET(socket, &socketSet);
        }

        maxSocket = maxSocket < mainSocket ? mainSocket : maxSocket;
    }
}

bool Server::isLoginUnique(const string& nickname) {
    return clientIds.find(nickname) == clientIds.end();
}

void Server::addClient(const shared_ptr<Client>& client) {
    this->clients.push_back(client);
    this->clientIds.emplace(client->getId());
}

const vector<shared_ptr<Lobby>>& Server::getLobbies() const {
    return lobbies;
}


void Server::sendLobbyList(shared_ptr<Client> client) {
    auto message = TCPData(RESPONSE);
    for (const auto& lobby : lobbies) {
        message.add(to_string(lobby->getId()), lobby->getState());
    }

    sendToClient(client.operator*(), message.serialize());
}

void Server::sendLoginUnique(int socket, bool isUnique) {
    auto message = TCPData(RESPONSE);
    message.add("isUnique", to_string(isUnique));
    sendToClient(socket, message.serialize());
}

bool Server::sendToClient(Client& client, const string& data) {
    return sendToClient(client.getFileDescriptor(), data);
}

bool Server::sendToClient(int socket, const string& data) {
    return send(socket, data.c_str(), data.length(), 0) > 0;
}

void Server::sendLobbyJoinable(Client& client, bool joinable) {
    auto response = TCPData(RESPONSE);
    response.add(IS_JOINABLE, to_string(joinable));
    sendToClient(client, response.serialize());
}

shared_ptr<Lobby> Server::getLobby(int id) {
    for (const auto& lobby: lobbies) {
        if (lobby->getId() == id) {
            return lobby;
        }
    }
    return nullptr;
}

shared_ptr<Client> Server::getClient(int socket) {
    for (const auto& client : clients) {
        if (client->getFileDescriptor() == socket) {
            return client;
        }
    }
    return nullptr;
}




