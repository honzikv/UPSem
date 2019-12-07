//
// Created by itznukee on 11/20/19.
//

#include "Server.h"
#include "serialization/Deserializer.h"
#include "communication/MessageSender.h"
#include "communication/MessageReceiver.h"

Server::Server(int port, int lobbyCount) {

    //Nastaveni filedescriptoru socketu serveru
    if ((fileDescriptor = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        cerr << "Error, OS could not create socket for the Server, try again" << endl;
        exit(EXIT_FAILURE);
    }


    //Nastaveni adresy serveru
    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htons(INADDR_ANY);
    address.sin_port = htons(port);

    //bind socketu
    if (bind(fileDescriptor, (struct sockaddr*) &address, sizeof(address)) < 0) {
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
    while (true) {

        if (listen(fileDescriptor, 3) < 0) {
            cerr << "Error while listening on Server socket" << endl;
            exit(EXIT_FAILURE);
        }

        if (auto newConnection = accept(fileDescriptor, (struct sockaddr*) &address,
                                        (socklen_t*) &addressLength) >= 0) {
            cout << "New connection" << endl;
            handleConnection(newConnection);
        }
    }
}

void Server::handleConnection(int socket) {

    auto const& server = this;
    auto clientThread = thread([server, socket] {
        cout << "Handling new client" << endl;

        auto messageSender = MessageSender(socket, (Server&) server);
        auto messageReceiver = MessageReceiver(socket, (Server&) server);

        auto username = messageReceiver.getUsername();
        if (username.empty()) {
            messageSender.sendLoginUnique(false);
            return; //TODO end connection
        }

        messageSender.sendLoginUnique(true);
        messageSender.sendLobbies();

        while (true) {

            auto selectedLobby = messageReceiver.getSelectedLobby();
            if (selectedLobby == INT32_MIN) {
                messageSender.sendLobbyNotJoinable();
            } else {
                auto client = make_shared<Client>(socket, username);
                server->addClient(client);
                server->getLobbies().at(selectedLobby)->addClient(client);
                return;
            }
        }

    });

    clientThread.join();
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





