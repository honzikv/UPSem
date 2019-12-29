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

        void handleMessage(const shared_ptr<Client>& client, const shared_ptr<TCPData>&);

        void handleResponse(const shared_ptr<Client>& client, const shared_ptr<TCPData>&);

        void handleRequest(const shared_ptr<Client>& client, const shared_ptr<TCPData>&);

        void sendShowPlayerConnectedRequest(const shared_ptr<Client>& client, const string& connectedPlayer);

        void sendShowPlayerDisconnectedRequest(const shared_ptr<Client>& client, const string& disconnectedPlayer);

        void sendMessage(int socket, const string& message);

        void sendUpdatePlayerListRequest(const shared_ptr<Client>& client);

        void sendPrepareGameSceneRequest(const shared_ptr<Client>& client);

        void sendLobbyStartFailed(const shared_ptr<Client>& client);

        void sendPlayerHand(const shared_ptr<Client>& client, bool isDealer);
};


#endif //UPSEM_LOBBYMESSAGEHANDLER_H
