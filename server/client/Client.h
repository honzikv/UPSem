//
// Created by itznukee on 11/20/19.
//

#ifndef UPSEM_CLIENT_H
#define UPSEM_CLIENT_H

#include <cstdlib>
#include <string>
#include <netinet/in.h>
#include <atomic>
#include <chrono>
#include <utility>
#include <vector>
#include <memory>
#include "player/PlayerInfo.h"

using namespace std;

class Client {

        /**
         * Id klienta, tzn. jeho username
         */
        const string username;

        /**
         * Cislo socketu klienta
         */
        int clientSocket;

        /**
         * Flag, zda-li uzivatel zvolil ze je pripraven pro start hry
         */
        bool ready = false;

        /**
         * Urcuje, zda-li je hrac disconnected
         */
        bool disconnected = false;

        /**
         * Flag pro server pro zjisteni, zda-li ma s klientem komunikovat
         */
        bool inLobby = false;

        int lobbyId = -1;

        chrono::time_point<chrono::system_clock> lastMessageReceived;

        shared_ptr<PlayerInfo> playerInfo;

    public:
        Client(string id, int fileDescriptor);

        string toString();

        int getLobbyId() const;

        void setLobbyId(int lobbyId);

        void setClientSocket(int clientSocket);

        bool isReady() const;

        bool operator==(const Client& anotherClient) const;

        bool operator!=(const Client& anotherClient) const;

        const string& getUsername() const;

        int getClientSocket() const;

        void setReady(bool hasVoted);

        void updateLastMessageReceived();

        void setDisconnected(bool disconnected);

        const chrono::time_point<chrono::system_clock>& getLastMessageReceived() const;

        const shared_ptr<PlayerInfo>& getPlayerInfo() const;

        void createPlayerInfo(int bet);
};


#endif //UPSEM_CLIENT_H
