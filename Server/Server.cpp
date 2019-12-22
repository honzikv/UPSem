//
// Created by itznukee on 11/20/19.
//

#include "Server.h"

Server::Server(int port, int lobbyCount) {

    //Nastaveni filedescriptoru socketu serveru
    if ((masterSocket = socket(AF_INET, SOCK_STREAM, 0)) <= 0) {
        cerr << "Error, OS could not create socket for the Server, try again" << endl;
        exit(EXIT_FAILURE);
    }


    //Nastaveni adresy serveru
    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_port = htons(port);

    //bind socketu
    if (bind(masterSocket, (struct sockaddr*) &address, sizeof(address)) < 0) {
        cerr << "Error, OS could not bind socket, try again" << endl;
        exit(EXIT_FAILURE);
    }
    cout << "Successfully bound socket to the server, ready to run." << endl;

    for (auto i = 0; i < lobbyCount; i++) {
        lobbies.push_back(make_shared<Lobby>(MAX_CLIENTS_PER_LOBBY, i));
    }
}

void Server::run() {

    if (listen(masterSocket, 3) < 0) {
        cerr << "Error while listening on server socket " << endl;
        exit(EXIT_FAILURE);
    }

    int addrlen = sizeof(address);

    fd_set readfds;
    auto maxSocket = 0;
    auto clientSockets = vector<int>();
    char buffer[MAX_BUFFER_SIZE_BYTES];
    timeval timeout;
    timeout.tv_sec = 5;
    timeout.tv_usec = 0;

    while (true) {
        FD_ZERO(&readfds);
        FD_SET(masterSocket, &readfds);
        maxSocket = masterSocket;

        for (const auto& clientSocket : clientSockets) {
            if (clientSocket > maxSocket) {
                maxSocket = clientSocket;
            }

            if (clientSocket > 0) {
                FD_SET(clientSocket, &readfds);
            }
        }

        auto activity = select(maxSocket + 1, &readfds, NULL, NULL, &timeout);

        if (FD_ISSET(masterSocket, &readfds)) {
            int newClient = accept(masterSocket, (struct sockaddr*) &address, (socklen_t*) &addrlen);

            if (newClient < 0) {
                cerr << "Error while accepting client" << endl;
                exit(EXIT_FAILURE);
            }

            cout << "New client connected" << endl; //todo log
            clientSockets.push_back(newClient);
        }

            //Iterace pres vsechny klientske sockety
        else {
            for (auto i = clientSockets.begin(); i != clientSockets.end(); i++) {
                //ziskani reference
                cout << to_string(i.operator*()) << endl;
                auto clientSocket = i.operator*();

                auto bytesRead = read(clientSocket, buffer, MAX_BUFFER_SIZE_BYTES - 1);
                cout<< bytesRead << endl;

                if (bytesRead <= 0) {
                    cout << clientSocket << " removed" << endl;
                    clientSockets.erase(i);
                    close(clientSocket);
                    continue;
                }

                auto message = TCPData(buffer);
                auto requestValue = message.valueOf(REQUEST);
                auto client = getClient(clientSocket);

                if (client == nullptr) {
                    auto username = message.valueOf(USERNAME);
                    if (isLoginUnique(username)) {
                        sendLoginUnique(clientSocket, true);
                        clients.push_back(make_shared<Client>(username, clientSocket));
                    }
                    else {
                        sendLoginUnique(clientSocket, false);
                    }
                }

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
                        clientSockets.erase(i);
                    } else {
                        sendLobbyJoinable(client.operator*(), false);
                    }
                }
            }
        }
    }
}

void handle() {
//    auto requestValue = message.valueOf(REQUEST);
//
//    if (requestValue == LOBBY_LIST) {
//        sendLobbyList(client);
//    }
//
//    if (requestValue == JOIN_LOBBY) {
//        auto lobby = getLobby(stoi(message.valueOf(LOBBY_ID)));
//        if (lobby == nullptr) {
//            throw exception();
//        }
//
//        if (lobby->isJoinable()) {
//            lobby->addClient(client);
//            sendLobbyJoinable(client.operator*(), true);
//            //Klienta připojíme do lobby a odstraníme ho z aktivních socketů selectu
//            sockets.erase(remove(sockets.begin(), sockets.end(), socket), sockets.end());
//            break;
//        } else {
//            sendLobbyJoinable(client.operator*(), false);
//        }
//    }
}

bool Server::isLoginUnique(const string& nickname) {
    return clientIds.find(nickname) == clientIds.end();
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




