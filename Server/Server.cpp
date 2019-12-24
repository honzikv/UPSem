//
// Created by itznukee on 11/20/19.
//

#include "Server.h"
#include "MessageHandler.h"

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

    this->messageHandler = make_unique<MessageHandler>(*this);
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
    timeout.tv_usec = 300;

    while (true) {
        FD_ZERO(&readfds);
        FD_SET(masterSocket, &readfds);
        maxSocket = masterSocket;

        setMaxSocket(clientSockets, readfds, maxSocket);
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
                    auto message = TCPData(buffer);
                    auto client = getClient(clientSocket);

                    if (client == nullptr) {
                    } else {
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

void Server::kickClient(Client& client) {

}

const shared_ptr<Client>& Server::getClient(int socket) {
    for (const auto& clientPtr : clients) {
        if (clientPtr->getFileDescriptor() == socket) {
            return clientPtr;
        }
    }

    return nullptr;
}

const shared_ptr<Lobby>& Server::getLobby(int lobbyId) {
    for (const auto& lobbyPtr : lobbies) {
        if (lobbyPtr->getId() == lobbyId) {
            return lobbyPtr;
        }
    }

    return nullptr;
}

const vector<shared_ptr<Lobby>>& Server::getLobbies() const {
    return lobbies;
}

bool Server::isLoginUnique(string basicString) {
    return false;
}




