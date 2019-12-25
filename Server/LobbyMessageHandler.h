//
// Created by itznu on 12/25/2019.
//

#ifndef UPSEM_LOBBYMESSAGEHANDLER_H
#define UPSEM_LOBBYMESSAGEHANDLER_H


#include "Client.h"
#include "serialization/TCPData.h"

class Lobby;

class LobbyMessageHandler {

        Lobby& lobby;

    public:
        LobbyMessageHandler(Lobby& lobby);

        void handleMessage(shared_ptr<Client>& client, TCPData& message);

        void pingBack(shared_ptr<Client>& sharedPtr);

        void handleResponse(shared_ptr<Client>& client, TCPData& message);

        void handleRequest(shared_ptr<Client>& client, TCPData& message);
};


#endif //UPSEM_LOBBYMESSAGEHANDLER_H
