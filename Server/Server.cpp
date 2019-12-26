//
// Created by itznukee on 11/20/19.
//

#include "Server.h"
#include "lobby/Lobby.h"
#include "communication/MessageHandler.h"

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
        cout << i << endl;
    }
    createThreads();

    this->messageHandler = make_shared<MessageHandler>(*this);
}

void Server::run() {

    if (listen(masterSocket, 3) < 0) {
        cerr << "Error while listening on server socket " << endl;
        exit(EXIT_FAILURE);
    }

    int addrlen = sizeof(address);

    //filedeskriptory socketu
    fd_set readfds;

    //maximalni socket
    auto maxSocket = 0;

    //list se sockety klientu
    auto clientSockets = vector<int>();

    //buffer pro přečtenou zprávu
    char buffer[MAX_BUFFER_SIZE_BYTES];

    //timeout klienta
    timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = 3000;

    while (true) {
        FD_ZERO(&readfds);
        FD_SET(masterSocket, &readfds);
        maxSocket = masterSocket;

        setMaxSocket(clientSockets, readfds, maxSocket);
        auto activity = select(maxSocket + 1, &readfds, NULL, NULL, NULL);

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
                auto clientSocket = i.operator*();

                memset(buffer, 0, MAX_BUFFER_SIZE_BYTES);
                auto bytesRead = read(clientSocket, buffer, MAX_BUFFER_SIZE_BYTES);

                if (bytesRead <= 0) {
                    cout << "client disconnected" << endl;
                    i = clientSockets.erase(i);
                    close(clientSocket);
                    continue;
                }

                try {
                    cout << buffer << "\n";
                    auto message = make_shared<TCPData>(buffer);
                    auto client = getClient(clientSocket);

                    if (client == nullptr) {
                        messageHandler->handleSocketMessage(clientSocket, message);
                    } else {

                        bool handledByLobby = false;
                        for (auto const& lobby : lobbies) {
                            if (lobby->contains(client)) {
                                lobby->addNewMessage(message, client);
                                handledByLobby = true;
                                break;
                            }
                        }

                        if (!handledByLobby) {
                            messageHandler->handleClientMessage(client, message);
                        }
                    }

                }
                catch (exception&) {
                    cerr << "client send incorrect input, disconnecting" << endl;
                    i = clientSockets.erase(i);
                    close(clientSocket);
                }
            }

        }
    }
}

/**
 * Nastaví maximální socket
 * @param clientSockets vektor se sockety klientů
 * @param fileDescriptorSet set file descriptorů
 * @param maxSocket reference na maximální socket
 */
void Server::setMaxSocket(const vector<int>& clientSockets, fd_set& fileDescriptorSet, int& maxSocket) const {
    for (const auto& clientSocket : clientSockets) {
        if (clientSocket > maxSocket) {
            maxSocket = clientSocket;
        }

        if (clientSocket > 0) {
            FD_SET(clientSocket, &fileDescriptorSet);
        }
    }
}


void Server::kickClient(shared_ptr<Client> client) {

}

shared_ptr<Client> Server::getClient(int socket) {
    for (const auto& client : clients) {
        if (client->getClientSocket() == socket) {
            return client;
        }
    }

    return shared_ptr<Client>(nullptr);
}

shared_ptr<Lobby> Server::getLobby(int lobbyId) {
    for (const auto& lobbyPtr : lobbies) {
        if (lobbyPtr->getId() == lobbyId) {
            return lobbyPtr;
        }
    }

    return shared_ptr<Lobby>(nullptr);
}

const vector<shared_ptr<Lobby>>& Server::getLobbies() {
    return lobbies;
}

bool Server::isLoginUnique(string username) {

    for (const auto& client : clients) {
        if (client->getId() == username) {
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

int Server::getMasterSocket() const {
    return masterSocket;
}

void Server::addClient(const string& username, int clientSocket) {
    clients.push_back(make_unique<Client>(username,clientSocket));
}





