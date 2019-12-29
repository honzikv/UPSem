//
// Created by itznukee on 11/20/19.
//

#include "Server.h"
#include "lobby/Lobby.h"
#include "communication/MessageHandler.h"

Server::Server(int port, int lobbyCount) : port(port) {
    for (auto i = 0; i < lobbyCount; i++) {
        lobbies.push_back(make_shared<Lobby>(MAX_CLIENTS_PER_LOBBY, i));
    }
    createThreads();
    this->messageHandler = make_shared<MessageHandler>(*this);
    selectServer();
}


void Server::selectServer() {

    //create a master socket
    if ((serverSocketFileDescriptor = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        cerr << "Initializing socket failed, please try again later" << endl;
        exit(EXIT_FAILURE);
    }

    auto option = 1;
    //nastaveni socketu aby prijmal vice spojeni
    if (setsockopt(serverSocketFileDescriptor, SOL_SOCKET, SO_REUSEADDR, (char*) &option,
                   sizeof(option)) < 0) {
        cerr << "Setting socket option failed, please try again later" << endl;
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    //bind na port
    if (bind(serverSocketFileDescriptor, (struct sockaddr*) &address, sizeof(address)) < 0) {
        cerr << "Bind failed, please try again later" << endl;
        exit(EXIT_FAILURE);
    }


    if (listen(serverSocketFileDescriptor, 3) < 0) {
        cerr << "Listen failed, please try again later" << endl;
        exit(EXIT_FAILURE);
    }

    //accept the incoming connection
    int addrlen = sizeof(address);
    auto maxSocket = -1;
    timeval timeout;

    cout << "Server is ready to handle players " << endl;
    while (true) {
        maxSocket = setupFileDescriptors(maxSocket);

        //nastaveni timeoutu
        timeout.tv_sec = 600;
        timeout.tv_usec = 0;

        auto activity = select(maxSocket + 1, &fileDescriptorSet, NULL, NULL, &timeout);

        if ((activity < 0)) {
            cerr << "Select error, please try again later" << endl;
            exit(EXIT_FAILURE);
        }
        //Pokud je neco na server socketu, jedna se o novy pokus o pripojeni
        if (FD_ISSET(serverSocketFileDescriptor, &fileDescriptorSet)) {
            acceptNewClient(addrlen);
        }

        //Iterujeme kazdy socket klienta, jestli je co cist
        auto iterator = 0;
        while (iterator < clientSockets.size()) {
            auto clientSocket = clientSockets[iterator];
            char buffer[MAX_BUFFER_SIZE_BYTES];
            memset(buffer, 0, MAX_BUFFER_SIZE_BYTES);

            if (FD_ISSET(clientSocket, &fileDescriptorSet)) {

                auto readResult = read(clientSocket, buffer, MAX_BUFFER_SIZE_BYTES);

                if (readResult == 0) {
                    //Pokud je vysledek 0 - End of Stream nebo -1 Exception, uzavreme socket
                    cerr << "Client was disconnected" << endl;
                    closeConnection(clientSocket);
                } else {
                    //jinak predame zpravu message handleru
                    try {
                        auto bufferAsString = string(buffer);
                        auto messageVector = vector<string>();
                        auto stringStream = stringstream(bufferAsString);

                        string temp;
                        while (getline(stringStream, temp, '\n')) {
                            messageVector.push_back(temp);
                        }

                        for (const auto& messageFromBuffer : messageVector) {
                            cout << messageFromBuffer << endl;
                            auto message = make_shared<TCPData>(messageFromBuffer);
                            messageHandler->handleMessage(clientSocket, message);
                        }
                    }
                    catch (exception&) {
                        cerr << "client sent incorrect input, disconnecting" << endl;
                        closeConnection(clientSocket);
                    }

                }
            }

            iterator++;
        }

        for (auto socketToClose : socketsToClose) {
            close(socketToClose);
            cout << "connection closed " << socketToClose << endl;
            clientSockets.erase(remove(
                    clientSockets.begin(), clientSockets.end(), socketToClose), clientSockets.end());

            auto client = getClientBySocket(socketToClose);
            if (client != nullptr) {
                client->setClientSocket(-1);

                auto lobby = getLobby(client->getLobbyId());
                if (lobby != nullptr) {
                    lobby->removeClient(client);
                }
            }
        }
        socketsToClose.clear();

        auto currentTime = chrono::system_clock::now();
        for (const auto& client : clients) {
            auto durationMillis = chrono::duration<double, milli>(currentTime - client->getLastMessageReceived()).count();

            if (durationMillis > MAX_TIMEOUT_BEFORE_DISCONNECT_MS) {
                client->setDisconnected(true);
            }

            if (durationMillis > MAX_TIMEOUT_BEFORE_REMOVED_MS) {
                removeClient(client);
            }
        }
    }
}

void Server::acceptNewClient(int& addressLength) {
    auto acceptResult = accept(serverSocketFileDescriptor, (sockaddr*) &clientAddress,
                               (socklen_t*) &addressLength);

    if (acceptResult < 0) {
        cerr << "Error while accepting client" << endl;
    } else {
        cout << "New connection socket " << acceptResult << " from ip"
             << inet_ntoa(clientAddress.sin_addr) << ":" << ntohs(clientAddress.sin_port) << endl;
        clientSockets.push_back(acceptResult);
    }
}

int Server::setupFileDescriptors(int maxSocket) {
    //vynulovani filedescriptor setu
    FD_ZERO(&fileDescriptorSet);
    //pridani socketu serveru do filedescriptor setu
    FD_SET(serverSocketFileDescriptor, &fileDescriptorSet);
    maxSocket = serverSocketFileDescriptor;

    for (const auto& clientSocket : clientSockets) {
        FD_SET(clientSocket, &fileDescriptorSet);

        if (clientSocket > maxSocket) {
            maxSocket = clientSocket;
        }
    }
    return maxSocket;
}


shared_ptr<Client> Server::getClientBySocket(int socket) {
    for (const auto& client : clients) {
        if (client->getClientSocket() == socket) {
            return client;
        }
    }

    return nullptr;
}

shared_ptr<Lobby> Server::getLobby(int lobbyId) {
    for (const auto& lobbyPtr : lobbies) {
        if (lobbyPtr->getId() == lobbyId) {
            return lobbyPtr;
        }
    }

    return nullptr;
}

const vector<shared_ptr<Lobby>>& Server::getLobbies() {
    return lobbies;
}

bool Server::isLoginUnique(const string& username) {

    for (const auto& client : clients) {
        if (client->getUsername() == username) {
            return false;
        }
    }
    return true;

}

bool Server::isLobbyJoinable(int lobbyId) {

    for (const auto& lobby : lobbies) {
        if (lobby->getId() == lobbyId) {
            return lobby->isJoinable();
        }
    }
    return false;
}

void lobbyFunction(shared_ptr<Lobby> lobby, Server& server) {

    while (true) {
        lobby->handleLobby();
        lobby->handleGameState();

        this_thread::sleep_for(chrono::milliseconds(LOBBY_THREAD_SLEEP_MS));
    }
}

void Server::createThreads() {

    auto& server = *this;
    for (const auto& lobbySharedPtr : lobbies) {
        auto lobbyThread = thread(lobbyFunction, lobbySharedPtr, ref(server));
        lobbyThread.detach();
        lobbyThreads.push_back(move(lobbyThread));
    }
}

void Server::registerClient(const string& username, int clientSocket) {
    clients.push_back(make_unique<Client>(username, clientSocket));
}

shared_ptr<Client> Server::getClientByUsername(const string& username) {

    for (const auto& client : clients) {
        if (client->getUsername() == username) {
            return client;
        }
    }

    return nullptr;
}

void Server::closeConnection(int clientSocket) {
    socketsToClose.insert(clientSocket);
}

shared_ptr<Lobby> Server::getLobby(const shared_ptr<Client>& client) {

    for (const auto& lobby : lobbies) {
        if (lobby->contains(client))
            return lobby;
    }

    return nullptr;
}

void Server::removeClient(const shared_ptr<Client>& client) {
    clients.erase(remove(clients.begin(), clients.end(), client), clients.end());

    if (client->getClientSocket() != -1) {
        close(client->getClientSocket());
        clientSockets.erase(remove(
                clientSockets.begin(), clientSockets.end(), client->getClientSocket()), clientSockets.end());
    }

    //Pokud je nahodou klient v lobby odstranime ho z lobby
    if (client->isInLobby() && getLobby(client->getLobbyId()) != nullptr) {
        getLobby(client->getLobbyId())->removeClient(client);
    }
    cout << "data of client " << client->getUsername() << " has been removed" << endl;
}

