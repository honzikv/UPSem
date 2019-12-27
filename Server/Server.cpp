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

    auto opt = 1;
    //nastaveni socketu aby prijmal vice spojeni
    if (setsockopt(serverSocketFileDescriptor, SOL_SOCKET, SO_REUSEADDR, (char*) &opt,
                   sizeof(opt)) < 0) {
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

    //buffer pro zpravy
    char buffer[MAX_BUFFER_SIZE_BYTES];

    cout << "Server is ready to handle clients " << endl;
    while (true) {
        //vynulovani filedescriptor setu
        FD_ZERO(&fileDescriptorSet);

        //pridani socketu serveru do filedescriptor setu
        FD_SET(serverSocketFileDescriptor, &fileDescriptorSet);
        maxSocket = serverSocketFileDescriptor;

        for (const auto& clientSocket : clientSockets) {
            FD_SET(clientSocket,&fileDescriptorSet);

            if (clientSocket > maxSocket) {
                maxSocket = clientSocket;
            }
        }

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
            auto acceptResult = accept(serverSocketFileDescriptor, (sockaddr*) &clientAddress,
                                       (socklen_t*) &addrlen);

            if (acceptResult < 0) {
                cerr << "Error while accepting client" << endl;
            } else {
                cout << "New connection from "
                << inet_ntoa(clientAddress.sin_addr) << ":" << ntohs(clientAddress.sin_port) << endl;
                clientSockets.push_back(acceptResult);
            }
        }

        //Iterujeme kazdy socket klienta, jestli je co cist
        auto iterator = 0;
        while (iterator < clientSockets.size()) {
            auto clientSocket = clientSockets[iterator];
            if (FD_ISSET(clientSocket, &fileDescriptorSet)) {

                memset(buffer, 0, MAX_BUFFER_SIZE_BYTES);
                auto readResult = read(clientSocket, buffer, MAX_BUFFER_SIZE_BYTES);

                if (readResult == 0) {
                    cerr << "Client was disconnected" << endl;
                    close(clientSocket);
                    clientSockets.erase(clientSockets.begin() + iterator);
                    continue;
                } else {
                    try {
                        auto bufferString = string(buffer);
                        auto messageList = vector<string>();
                        auto stringStream = stringstream(bufferString);

                        string temp;
                        while (getline(stringStream, temp, '\n')) {
                            messageList.push_back(temp);
                        }
                        for (auto string : messageList) {
                            auto message = make_shared<TCPData>(string);

                            auto client = getClient(clientSocket);

                            if (client == nullptr) {

                                auto keepConnection = messageHandler->handleSocketMessage(clientSocket, message);

                                if (!keepConnection) {
                                    close(clientSocket);
                                    clientSockets.erase(clientSockets.begin() + iterator);
                                    continue;
                                }

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

                    }
                    catch (exception&) {
                        cerr << "client sent incorrect input, disconnecting" << endl;
                        close(clientSocket);
                        clientSockets.erase(clientSockets.begin() + iterator);
                        continue;
                    }

                    cout << endl;
                }
            }

            iterator++;
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

void Server::addClient(const string& username, int clientSocket) {
    clients.push_back(make_unique<Client>(username, clientSocket));
}

void Server::init() {

    serverSocketFileDescriptor = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocketFileDescriptor == -1) {
        cerr << "Creating socket failed, please try to restart program with different port" << endl;
        exit(EXIT_FAILURE);
    }

    auto socketOption = 1;
    auto socketOpResult = setsockopt(serverSocketFileDescriptor, SOL_SOCKET, SO_REUSEADDR, &socketOption,
                                     sizeof(socketOption));
    if (socketOpResult == -1) {
        cerr << "Error setting socket option, please try again with different port" << endl;
        exit(EXIT_FAILURE);
    }

    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_port = htons(port);

    auto bindResult = bind(serverSocketFileDescriptor, (struct sockaddr*) &address, sizeof(address));
    if (bindResult == -1) {
        cerr << "Error while binding socket, please try again with different port" << endl;
        exit(EXIT_FAILURE);
    }

    auto listenResult = listen(serverSocketFileDescriptor, 3);
    if (listenResult == -1) {
        cerr << "Error while listening" << endl;
        exit(EXIT_FAILURE);
    }

}

void Server::launch() {

    timeval timeout;
    FD_ZERO(&fileDescriptorSet);
    FD_SET(serverSocketFileDescriptor, &fileDescriptorSet);

    auto maxSocket = serverSocketFileDescriptor;
    int addrlen = sizeof(clientAddress);
    auto clientSockets = vector<int>();

    char buffer[MAX_BUFFER_SIZE_BYTES];

    timeout.tv_sec = 5;
    timeout.tv_usec = 0;

    for (const auto& clientSocket : clientSockets) {
        if (clientSocket > maxSocket) {
            maxSocket = clientSocket;
        }

        if (clientSocket > 0) {
            FD_SET(clientSocket, &fileDescriptorSet);
        }
    }

    auto activity = select(maxSocket + 1, &fileDescriptorSet, NULL, NULL, &timeout);

    if ((activity < 0) && (errno != EINTR)) {
        cerr << "Error in select function" << endl;
        exit(EXIT_FAILURE);
    }

    if (FD_ISSET(maxSocket, &fileDescriptorSet)) {
        auto newSocket = -1;
        if ((newSocket = accept(serverSocketFileDescriptor, (struct sockaddr*) &clientAddress,
                                (socklen_t*) &addrlen)) <
            0) {
            cerr << "Error while accepting client" << endl;
            exit(EXIT_FAILURE);
        }

        cout << "New client connected" << endl;
        clientSockets.push_back(newSocket);
    }

    auto iterator = clientSockets.begin();
    while (iterator != clientSockets.end()) {
        auto clientSocket = *iterator;

        //Pokud je filedescriptor nastaveny, precteme data
        if (FD_ISSET(clientSocket, &fileDescriptorSet)) {

            cout << "read some stuff" << endl;
            memset(buffer, 0, MAX_BUFFER_SIZE_BYTES);
            auto bytesRead = read(clientSocket, buffer, MAX_BUFFER_SIZE_BYTES);

            if (bytesRead <= 0) {
                cout << "client disconnected" << endl;
                iterator = clientSockets.erase(iterator);
                close(clientSocket);
                iterator++;
                continue;
            }

            try {
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
                cerr << "client sent incorrect input, disconnecting" << endl;
                iterator = clientSockets.erase(iterator);
                close(clientSocket);
            }
        }

        iterator++;
    }


}


