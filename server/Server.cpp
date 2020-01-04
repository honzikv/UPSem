//
// Created by itznukee on 11/20/19.
//

#include "Server.h"
#include "lobby/Lobby.h"
#include "communication/handlers/MessageHandler.h"

Server::Server(int port, int lobbyCount) : port(port) {
    for (auto i = 0; i < lobbyCount; i++) {
        lobbies.push_back(make_shared<Lobby>(MAX_CLIENTS_PER_LOBBY, i));
    }
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

    int addrlen = sizeof(address);
    auto maxSocket = -1;
    timeval timeout;

    cout << "server is ready to handle players " << endl;
    while (true) {
        maxSocket = setupFileDescriptors(maxSocket);

        //nastaveni timeoutu
        timeout.tv_sec = 30;
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

        /*
         * Iterace pres vektor klientu, pokud klient neco poslal, adekvatne zpracujeme zpravu, pokud se klient
         * odpojil, iterator socket zavre
         */
        auto iterator = 0;
        while (iterator < clientSockets.size()) {
            auto clientSocket = clientSockets[iterator];
            char buffer[MAX_BUFFER_SIZE_BYTES];
            memset(buffer, 0, MAX_BUFFER_SIZE_BYTES);

            if (FD_ISSET(clientSocket, &fileDescriptorSet)) {

                auto readResult = read(clientSocket, buffer, MAX_BUFFER_SIZE_BYTES);

                if (readResult <= 0) {
                    //pokud je vysledek 0 - End of Stream nebo -1 Exception, uzavreme socket
                    cout << "Client was disconnected" << endl;
                    //Preda socket do vektoru pro odpojeni - klient zustane zachovan
                    closeConnection(clientSocket);
                } else {
                    //jinak predame zpravu message handleru
                    try {
                        //teoreticky by se mohlo stat, ze buffer nacte vice nez jednu zpravu
                        auto bufferAsString = string(buffer);
                        auto messageVector = vector<string>();
                        auto stringStream = stringstream(bufferAsString);

                        /*
                         * klient odesila zpravy ve formatu {}\n, tzn. muzeme je rozdelit pomoci \n znaku,
                         * pokud je jakakoliv zprava poskozena, dojde k zavreni spojeni - parser zprav hodi
                         * DeserializationException a spojeni se ze strany serveru uzavre
                         */
                        string temp;
                        while (getline(stringStream, temp, '\n')) {
                            messageVector.push_back(temp);
                        }

                        for (const auto& messageFromBuffer : messageVector) {
                            auto message = make_shared<TCPData>(messageFromBuffer);
                            messageHandler->handleMessage(clientSocket, message);
                        }
                    }
                    catch (exception&) {
                        cerr << "client sent incorrect input, disconnecting" << endl;
                        closeConnection(clientSocket);
                        continue;
                    }

                }
            }

            iterator++;
        }

        disconnectClients();
        for (const auto& lobby : lobbies) {
            lobby->handleLobby();
        }
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

void Server::disconnectClients() {

    /*
     * Detekce, ze se klient odpojil se provede pomoci timeoutu - klient posila kazdych par sekund ping aby udrzel
     * spojeni se serverem + jine pozadavky. Po uplynuti timeoutu se u klienta nastavi pole disconnected na true,
     * pokud se neozve do delsi doby, jsou veskera data o klientovi odstranena.
     */
    auto currentTime = chrono::system_clock::now();
    for (const auto& client : clients) {
        auto durationMillis = chrono::duration<double, milli>(
                currentTime - client->getLastMessageReceived()).count();

        if (durationMillis > MAX_TIMEOUT_BEFORE_DISCONNECT_MS) {
            if (client->getClientSocket() != -1) {
                closeConnection(client->getClientSocket());
            }
        }
        if (durationMillis > MAX_TIMEOUT_BEFORE_REMOVED_MS) {
            //Pokud klient neni v lobby odstranime ho
            cout << "Data of client " << client->getUsername() << " has been removed" << endl;
            if (client->getLobbyId() == -1) {
                clients.erase(remove(clients.begin(), clients.end(), client), clients.end());
                continue;
            }

            //Pokud je lobby ve stavu, ze se nekona ani nepripravuje zadna hra, klienta odstrani
            auto lobby = getLobby(client->getLobbyId());
            if (lobby->getLobbyState() == LOBBY_STATE_WAITING) {
                lobby->removeClient(client);
            }
        }
    }

    /*
     * Sockety, ktere se maji zavrit se ukladaji do vectoru socketsToClose a zaviraji se az po probehnuti selectu,
     * pokud je pro socket zaregistrovany klient a klient byl ve hre, ostatnim klientum je zaslana zprava, ze se
     * klient odpojil.
     */
    for (auto socketToClose : socketsToClose) {
        close(socketToClose);
        cout << "connection closed " << socketToClose << endl;
        clientSockets.erase(remove(
                clientSockets.begin(), clientSockets.end(), socketToClose), clientSockets.end());

        auto client = getClientBySocket(socketToClose);
        if (client == nullptr) {
            continue;
        }

        client->setClientSocket(-1);

        auto lobby = getLobby(client->getLobbyId());
        if (lobby == nullptr) {
            continue;
        }

        auto lobbyState = lobby->getLobbyState();
        if (lobbyState == LOBBY_STATE_WAITING) {
            lobby->removeClient(client);
        } else {
            lobby->sendClientDisconnected(client);
        }
    }
    socketsToClose.clear();

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

bool Server::isLobbyJoinable(int lobbyId) {
    for (const auto& lobby : lobbies) {
        if (lobby->getId() == lobbyId) {
            return lobby->isJoinable();
        }
    }
    return false;
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

