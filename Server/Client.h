//
// Created by itznukee on 11/20/19.
//

#ifndef UPSEM_CLIENT_H
#define UPSEM_CLIENT_H

#include <cstdlib>
#include <string>
#include <netinet/in.h>

using namespace std;

class Client {

        /**
         * Id klienta, tzn. jeho username
         */
        const string id;

        /**
         * Cislo socketu klienta
         */
        int clientSocket;

        /**
         * Flag, zda-li uzivatel volil pro start hry, pokud alespon 50% uzivatelu volilo pro start lobby automaticky
         * spusti hru
         */
        bool voted = false;

        /**
         * Flag pro server pro zjisteni, zda-li ma s klientem komunikovat
         */
        bool isInLobby = false;

    public:
        bool hasVoted() const;

        bool operator==(const Client& anotherClient) const;

        bool operator!=(const Client& anotherClient) const;

    public:
        Client(string id, int fileDescriptor);

        const string& getId() const;

        int getClientSocket() const;

        void setHasVoted(bool hasVoted);

};


#endif //UPSEM_CLIENT_H
