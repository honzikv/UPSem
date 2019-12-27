//
// Created by nvk on 12/26/2019.
//

#ifndef UPSEM_MESSAGEHANDLER_H
#define UPSEM_MESSAGEHANDLER_H

#include "../serialization/TCPData.h"
#include "../Client.h"
#include "../lobby/Lobby.h"

class Server;

class MessageHandler {

        /**
         * Reference na server
         *
         */
        Server& server;


    public:
        explicit MessageHandler(Server& server);

        /**
         * Funkce pro prihlaseni klienta - reconnect a login.
         * @param clientSocket socket klienta
         * @param message zprava
         * @return vraci true, pokud byl klient uspesne pripojen, jinak se spojeni zavre
         */
        bool handleLogin(int clientSocket, const shared_ptr<TCPData>& message);

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


        void sendLobbyInfo(const shared_ptr<Client>& client, const shared_ptr<Lobby>& lobby);
};


#endif //UPSEM_MESSAGEHANDLER_H
