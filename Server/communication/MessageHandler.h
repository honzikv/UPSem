//
// Created by nvk on 12/26/2019.
//

#ifndef UPSEM_MESSAGEHANDLER_H
#define UPSEM_MESSAGEHANDLER_H

#include "../serialization/TCPData.h"
#include "../Client.h"

class Server;

class MessageHandler {

        /**
         * Reference na server
         *
         */
        Server& server;


    public:
        explicit MessageHandler(Server& server);

        void handleSocketMessage(int clientSocket, const shared_ptr<TCPData>& message);


        void handleClientMessage(const shared_ptr<Client>& client, const shared_ptr<TCPData>& message);


    private:
        void handleClientRequest(const shared_ptr<Client>& client, const shared_ptr<TCPData>& message);

        void handleClientResponse(const shared_ptr<Client>& client, const shared_ptr<TCPData>& message);

        static void pingBack(const shared_ptr<Client>& client);

        static void sendMessage(int socket, const string& message);

        static void sendUsernameUnique(int clientSocket, bool isUnique);

        void sendLobbyList(const shared_ptr<Client>& client);

        static void sendLobbyJoinable(const shared_ptr<Client>& client, bool joinable, int lobbyId);

        static void sendClientNotFound(int clientSocket);

        static void sendClientReconnected(int clientSocket);


};


#endif //UPSEM_MESSAGEHANDLER_H
