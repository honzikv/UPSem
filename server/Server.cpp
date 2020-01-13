//
// Created by itznukee on 11/20/19.
//

#include "Server.h"
#include "lobby/Lobby.h"
#include "communication/handlers/MessageHandler.h"

Server::Server(const string &ip, int port, int lobbyCount) : ip(ip), port(port) {

    for (auto i = 0; i < lobbyCount; i++) {
        lobbies.push_back(make_shared<Lobby>(MAX_CLIENTS_PER_LOBBY, i));
    }
    this->messageHandler = make_shared<MessageHandler>(*this);
    selectServer();
}

void Server::selectServer() {

    //vytvoreni master socketu serveru
    if ((masterSocket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        cerr << "Initializing socket failed, please try again later" << endl;
        exit(EXIT_FAILURE);
    }

    auto option = 1;
    //nastaveni socketu aby prijmal vice spojeni
    if (setsockopt(masterSocket, SOL_SOCKET, SO_REUSEADDR, (char *) &option,
                   sizeof(option)) < 0) {
        cerr << "Setting socket option failed, please try again later" << endl;
        exit(EXIT_FAILURE);
    }

    //nastaveni adresy
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(ip.c_str());
    address.sin_port = htons(port);

    //bind na port
    if (bind(masterSocket, (struct sockaddr *) &address, sizeof(address)) < 0) {
        cerr << "Bind failed, please try again later" << endl;
        exit(EXIT_FAILURE);
    }

    //listen
    if (listen(masterSocket, 3) < 0) {
        cerr << "Listen failed, please try again later" << endl;
        exit(EXIT_FAILURE);
    }

    int addrlen = sizeof(address);
    timeval timeout{};

    cout << "Server successfully launched @ " << ip << ":" << port << endl;
    cout << "Server is ready to handle players " << endl;

    fd_set tests;
    FD_ZERO(&clientFileDescriptors);
    FD_SET(masterSocket, &clientFileDescriptors);
    while (true) {
        //kopie fdSetu do noveho protoze select neni stabilni a upravi ho
        tests = clientFileDescriptors;

        //nastaveni timeoutu
        timeout.tv_sec = 2;
        timeout.tv_usec = 0;

        //nasledny block selectu
        select(FD_SETSIZE, &tests, NULL, NULL, &timeout);
        auto a2read = 0;
        char buffer[MAX_BUFFER_SIZE_BYTES];
        for (auto fileDescriptor = 3; fileDescriptor < FD_SETSIZE; fileDescriptor++) {
            //pokud je fileDescriptor citelny
            if (FD_ISSET(fileDescriptor, &tests)) {
                //Pokud je neco na server socketu, jedna se o novy pokus o pripojeni
                if (fileDescriptor == masterSocket) {
                    auto clientFileDescriptor = accept(masterSocket, (sockaddr *) &clientAddress,
                                                       (socklen_t *) &addrlen);
                    FD_SET(clientFileDescriptor, &clientFileDescriptors);
                    if (clientFileDescriptor < 0) {
                        cerr << "Error while accepting client" << endl;
                    } else {
                        cout << "New connection socket id is " << clientFileDescriptor << " from ip"
                             << inet_ntoa(clientAddress.sin_addr) << ":" << ntohs(clientAddress.sin_port) << endl;
                    }
                } else {
                    //zjisteni poctu bytu ke cteni
                    ioctl(fileDescriptor, FIONREAD, &a2read);
                    if (a2read > 0) {
                        memset(buffer, 0, MAX_BUFFER_SIZE_BYTES);
                        read(fileDescriptor, buffer, MAX_BUFFER_SIZE_BYTES);
                        //jinak predame zpravu message handleru
                        try {
                            //teoreticky by se mohlo stat, ze buffer nacte vice nez jednu zpravu
                            auto bufferAsString = string(buffer);
                            auto messageVector = vector<string>();
                            auto stringStream = stringstream(bufferAsString);

                            /*
                             * klient odesila zpravy ve formatu {}\n, tzn. muzeme je rozdelit pomoci \n znaku,
                             * pokud je jakakoliv zprava poskozena, dojde k zavreni spojeni - parser zprav hodi
                             * SerializationException a spojeni se ze strany serveru uzavre
                             */
                            string temp;
                            while (getline(stringStream, temp, '\n')) {
                                messageVector.push_back(temp);
                            }

                            for (const auto &messageFromBuffer : messageVector) {
                                auto message = make_shared<TCPData>(messageFromBuffer);
                                messageHandler->handleMessage(fileDescriptor, message);
                            }
                        }
                        catch (exception &) {
                            cerr << "client sent incorrect input, disconnecting" << endl;
                            //uzavreme spojeni, nevyhovuje protokolu
                            closeConnection(fileDescriptor);
                        }
                        //jinak uzavreme spojeni - nevyhovuje protokolu
                    } else {
                        closeConnection(fileDescriptor);
                    }
                }
            }
        }

        //odpojeni vsech klientu, kteri porusili protokol nebo jsou timed out
        disconnectClients();

        //lobby handling
        for (const auto &lobby : lobbies) {
            lobby->handleLobby();
        }
    }
}

void Server::disconnectClients() {

    /*
     * Detekce, ze se klient odpojil se provede pomoci timeoutu - klient posila kazdych par sekund ping aby udrzel
     * spojeni se serverem + jine pozadavky. Po uplynuti timeoutu se u klienta nastavi pole disconnected na true,
     * pokud se neozve do delsi doby, jsou veskera data o klientovi odstranena.
     */
    auto currentTime = system_clock::now();
    for (const auto &client : clients) {
        auto durationMillis = duration<double, milli>(
                currentTime - client->getLastMessageReceived()).count();

        if (durationMillis > MAX_TIMEOUT_BEFORE_DISCONNECT_MS && client->getClientSocket() != -1) {
            cout << "Client " << client->getUsername() << " with socket no. " << client->getClientSocket()
                 << " has reached timeout and will be disconnected" << endl;
            closeConnection(client->getClientSocket());
        }
    }

    //Smazani klientu, kteri se dlouho nepripojili pro snizeni pametove narocnosti
    currentTime = system_clock::now();
    auto clientsIt = clients.begin();
    while (clientsIt != clients.end()) {
        auto client = *clientsIt;
        auto durationMillis = duration<double, milli>(
                currentTime - client->getLastMessageReceived()).count();

        if (durationMillis > MAX_TIMEOUT_BEFORE_REMOVED_MS) {
            //Pokud klient neni v lobby odstranime ho
            cout << "Data of client " << client->getUsername() << " has been removed" << endl;
            if (client->getLobbyId() != -1) {
                auto lobby = getLobby(client->getLobbyId());
                if (lobby->getLobbyState() == LOBBY_STATE_WAITING) {
                    lobby->removeClient(client);
                }
            }
            clients.erase(clientsIt);
            continue;
        }
        clientsIt++;
    }

    /*
     * Sockety, ktere se maji zavrit se ukladaji do vectoru socketsToClose a zaviraji se az po probehnuti selectu,
     * pokud je pro socket zaregistrovany klient a klient byl ve hre, ostatnim klientum je zaslana zprava, ze se
     * klient odpojil.
     */
    for (auto socketToClose : socketsToClose) {
        close(socketToClose);
        cout << "Communication with socket " << socketToClose << " was closed" << endl;
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
            lobby->addDisconnectedClient(client);
            lobby->sendClientDisconnected(client);
        }
    }
    socketsToClose.clear();

}

shared_ptr<Client> Server::getClientBySocket(int socket) {
    for (const auto &client : clients) {
        if (client->getClientSocket() == socket) {
            return client;
        }
    }
    return nullptr;
}

shared_ptr<Lobby> Server::getLobby(int lobbyId) {
    for (const auto &lobbyPtr : lobbies) {
        if (lobbyPtr->getId() == lobbyId) {
            return lobbyPtr;
        }
    }
    return nullptr;
}

const vector<shared_ptr<Lobby>> &Server::getLobbies() {
    return lobbies;
}

bool Server::isLobbyJoinable(int lobbyId) {
    for (const auto &lobby : lobbies) {
        if (lobby->getId() == lobbyId) {
            return lobby->isJoinable();
        }
    }
    return false;
}

void Server::registerClient(const string &username, int clientSocket) {
    clients.push_back(make_unique<Client>(username, clientSocket));
}

shared_ptr<Client> Server::getClientByUsername(const string &username) {
    for (const auto &client : clients) {
        if (client->getUsername() == username) {
            return client;
        }
    }
    return nullptr;
}

void Server::closeConnection(int clientSocket) {
    socketsToClose.insert(clientSocket);
    FD_CLR(clientSocket, &clientFileDescriptors);
}

Server::~Server() {
    /*
     * Destruktor odpoji vsechny klienty a master socket - teoreticky to OS udela za nas (resp. WSL to udelalo vzdy)
     * a nema smysl resit, ale jedna se o good practice
     */
    close(masterSocket);
    for (const auto &client : clients) {
        close(client->getClientSocket());
    }
}

