//
// Created by itznukee on 11/20/19.
//

#ifndef UPSEM_CLIENT_H
#define UPSEM_CLIENT_H

#include <cstdlib>
#include <string>
#include <netinet/in.h>
#include <atomic>

using namespace std;

class Client {

        /**
         * Id klienta, tzn. jeho username
         */
        const string username;

        /**
         * Cislo socketu klienta
         */
        atomic<int> clientSocket;

        /**
         * Flag, zda-li uzivatel volil pro start hry, pokud alespon 50% uzivatelu volilo pro start lobby automaticky
         * spusti hru
         */
        bool voted = false;

        /**
         * Flag pro server pro zjisteni, zda-li ma s klientem komunikovat
         */
        bool inLobby = false;

        int lobbyId;


    public:
        int getLobbyId() const;

        void setLobbyId(int lobbyId);

        bool isInLobby() const;

        void setInLobby(bool inLobby);

        void setClientSocket(int clientSocket);

        bool hasVoted() const;

        bool operator==(const Client& anotherClient) const;

        bool operator!=(const Client& anotherClient) const;

        Client(string id, int fileDescriptor);

        const string& getUsername() const;

        int getClientSocket() const;

        void setHasVoted(bool hasVoted);

};


#endif //UPSEM_CLIENT_H
