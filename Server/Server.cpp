//
// Created by itznukee on 11/20/19.
//

#include "Server.h"


Server::Server(int port, int lobbyCount) : lobbyCount(lobbyCount) {
    this->clients = unordered_set(c)

    //Nastaveni filedescriptoru socketu serveru
    if ((this->fileDescriptor = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        cerr << "Error, OS could not create socket for the Server, try again" << endl;
        exit(EXIT_FAILURE);
    }

    //Nastaveni adresy serveru
    memset(&this->address, 0, sizeof(this->address));
    this->address.sin_family = AF_INET;
    this->address.sin_addr.s_addr = htons(INADDR_ANY);
    this->address.sin_port = htons(port);
    this->addressLength = sizeof(address);

    //bind socketu
    if (bind(this->fileDescriptor, (struct sockaddr *) &address, sizeof(this->address)) < 0) {
        cerr << "Error, OS could not bind socket, try again" << endl;
    }
    cout << "Successfully bound socket to Server, ready to run." << endl;
}

void Server::run() {

    cout << "Launching Server ..." << endl;

    //Vytvori lobbies podle zadaneho poctu
    for (auto i = 0; i < lobbyCount; i++) {
        lobbies.emplace_back(10);
    }

    while (true) {

        if (listen(fileDescriptor, 3) < 0) {
            cerr << "Error while listening on Server socket" << endl;
            exit(EXIT_FAILURE);
        }

        if (auto newConnection = accept(this->fileDescriptor, (struct sockaddr *) &this->address,
                                        (socklen_t *) &addressLength) >= 0) {
            handleConnection(newConnection);
        }
    }
}

void Server::handleConnection(int socket) {

    thread threadObj([]{
    });
}

const JSONParser &Server::getJsonParser() const {
    return jsonParser;
}

bool Server::isLoginUnique(const string nickname) {

    for (const auto& client : clients) {
        if (nickname == client.getId()) {
            return false;
        }
    }

    return true;
}




