//
// Created by itznu on 12/25/2019.
//

#ifndef UPSEM_LOBBYMESSAGEHANDLER_H
#define UPSEM_LOBBYMESSAGEHANDLER_H


#include "../Client.h"
#include "../Server.h"
#include "../serialization/TCPData.h"

class Lobby;

class LobbyMessageHandler {

        Lobby& lobby;

    public:
        explicit LobbyMessageHandler(Lobby& lobby);

        void handleMessage(shared_ptr<Client>& client, shared_ptr<TCPData>&);

        void handleResponse(shared_ptr<Client>& client, shared_ptr<TCPData>&);

        void handleRequest(shared_ptr<Client>& client, shared_ptr<TCPData>&);
};


#endif //UPSEM_LOBBYMESSAGEHANDLER_H
