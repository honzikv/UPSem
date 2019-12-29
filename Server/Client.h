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
#include "../Game/PlayerInfo.h"

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
        atomic<bool> voted = false;

        /**
         * Urcuje, zda-li je hrac disconnected
         */
        atomic<bool> disconnected = false;

        /**
         * Flag pro server pro zjisteni, zda-li ma s klientem komunikovat
         */
        atomic<bool> inLobby = false;

        atomic<int> lobbyId = -1;

        chrono::time_point<chrono::system_clock> lastMessageReceived;

        shared_ptr<PlayerInfo> playerInfo;

    public:
        Client(string id, int fileDescriptor);

        int getLobbyId() const;

        void setLobbyId(int lobbyId);

        bool isInLobby() const;

        void setInLobby(bool inLobby);

        void setClientSocket(int clientSocket);

        bool hasVoted() const;

        bool operator==(const Client& anotherClient) const;

        bool operator!=(const Client& anotherClient) const;

        const string& getUsername() const;

        int getClientSocket() const;

        void setHasVoted(bool hasVoted);

        void updateLastMessageReceived();

        void setDisconnected(bool disconnected);

        const chrono::time_point<chrono::system_clock>& getLastMessageReceived() const;

        const shared_ptr<PlayerInfo>& getPlayerInfo() const;

        void clearPlayerInfo();
};


#endif //UPSEM_CLIENT_H
