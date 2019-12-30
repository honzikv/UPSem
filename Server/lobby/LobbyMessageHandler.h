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

        void sendGameStartFailed(const shared_ptr<Client>& client);

        void sendClientDidntConfirm(const shared_ptr<Client>& client);

        void sendPlayersTurnRequest(const shared_ptr<Client>& client);

        void sendBoard(const shared_ptr<Client>& client, const vector<shared_ptr<Client>>& players,
                       const shared_ptr<Dealer>& dealer);

        void sendNotYourTurn(const shared_ptr<Client>& client);

        void sendPlayerTurn(const shared_ptr<TurnResult>& turnResult, const shared_ptr<Client>& client);

        void sendResults(const vector<shared_ptr<Client>>& clients, const shared_ptr<Dealer>& dealer);

        void sendShowLobbyRequest(const shared_ptr<Client>& client);
};


#endif //UPSEM_LOBBYMESSAGEHANDLER_H
