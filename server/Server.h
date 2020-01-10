//
// Created by itznukee on 11/20/19.
//

#ifndef UPSEM_SERVER_H
#define UPSEM_SERVER_H

#include <iostream>
#include <cstring>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstdlib>
#include <vector>
#include <unistd.h>
#include <cstdio>
#include <sys/select.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <unordered_set>
#include <algorithm>
#include <memory>
#include <csignal>
#include <sys/ioctl.h>

#include "client/Client.h"
#include "lobby/Lobby.h"
#include "communication/Constants.h"
#include "communication/serialization/TCPData.h"
#include "communication/Fields.h"
#include "communication/Values.h"

using namespace std;

class Lobby;

class MessageHandler;

/**
 * Trida, ktera uklada informace o serveru
 */
class Server {
        /*
         * Master socket pro select()
         */
        int masterSocket = 0;

        /**
         * Port serveru
         */
        const int port;

        /**
         * IP adresa serveru
         */
        const string ip;

        /**
         * Info o adrese serveru
         */
        struct sockaddr_in address{};

        /**
         * Info o adrese klienta pro select
         */
        struct sockaddr_in clientAddress{};

        /**
         * Set vsech file descriptoru
         */
        fd_set clientFileDescriptors{};

        /**
         * Seznam vsech klientu
         */
        vector<shared_ptr<Client>> clients;

        /**
         * Seznam vsech lobby
         */
        vector<shared_ptr<Lobby>> lobbies;

        /**
         * Set (pokud by byl prikaz zadan vice nez 1, aby se zbytecne nezaviral stejny file descriptor) socketu,
         * ktere se po selectu zavrou
         */
        unordered_set<int> socketsToClose;

        /**
         * Instance message handleru - zpracovani prijatych zprav
         */
        shared_ptr<MessageHandler> messageHandler;

    public:
        /**
         * Konstruktor pro vytvoreni serveru
         * @param ip ip adresa serveru
         * @param port port serveru
         * @param lobbyCount pocet hernich mistnosti
         */
        Server(const string& ip, int port, int lobbyCount);

        /**
         * Destruktor, ktery se zavola po ukonceni programu
         */
        virtual ~Server();

        /**
         * Najde klienta podle id file descriptoru socketu
         * @param socket id socketu
         * @return shared pointer na klienta nebo nullptr, pokud klient nebyl nalezen
         */
        shared_ptr<Client> getClientBySocket(int socket);

        /**
         * Najde a vrati referenci na lobby podle id lobby
         * @param lobbyId cele cislo >=0
         * @return shared pointer na lobby nebo nullptr, pokud lobby nebyla nalezena
         */
        shared_ptr<Lobby> getLobby(int lobbyId);

        /**
         * Vrati klienta podle jeho username, pokud neni nalezen vrati {@code nullptr}
         * @param username uzivatelske jmeno klienta
         * @return shared pointer klienta nebo nullptr pokud klient nebyl nalezen
         */
        shared_ptr<Client> getClientByUsername(const string& username);

        /**
         * Vrati vsechny herni mistnosti na serveru
         * @return referenci na vektor s hernimi mistnostmi
         */
        const vector<shared_ptr<Lobby>>& getLobbies();

        /**
         * Zjisti zda-li je lobby dostupna pro pripojeni
         * @param lobbyId id lobby
         * @return true pokud se do lobby lze pripojit, jinak false
         */
        bool isLobbyJoinable(int lobbyId);

        /**
         * Zaregistruje noveho klienta
         * @param username jmeno klienta
         * @param clientSocket aktualni socket klienta
         */
        void registerClient(const string& username, int clientSocket);

        /**
         * Prida do vektoru {@code socketsToClose} novy socket, ktery se po selectu a cteni zprav zavre
         * @param clientSocket cislo socketu
         */
        void closeConnection(int clientSocket);

        /**
         * Nekonecna smycka se selectem
         */
        void selectServer();

        /**
         * Odpoji vsechny klienty, kteri dlouho neodpovedeli nebo se maji odpojit
         */
        void disconnectClients();
};

#endif