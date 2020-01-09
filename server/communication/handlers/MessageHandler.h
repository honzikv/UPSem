//
// Created by nvk on 12/26/2019.
//

#ifndef UPSEM_MESSAGEHANDLER_H
#define UPSEM_MESSAGEHANDLER_H

#include "../../client/Client.h"
#include "../../lobby/Lobby.h"
#include "../serialization/TCPData.h"

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
         * Metoda pro prihlaseni klienta - reconnect a login.
         * @param clientSocket socket klienta
         * @param message zprava
         * @return vraci true, pokud byl klient uspesne pripojen, jinak se spojeni zavre
         */
        void handleMessage(int clientSocket, const shared_ptr<TCPData>& message);

    private:

        void pingBack(int clientSocket);

        void sendMessage(int socket, const string& message);

        void sendLobbyList(const shared_ptr<Client>& client);

        void handleRequest(int clientSocket, const shared_ptr<TCPData>& message);

        void sendLoginIsNew(int clientSocket);

        void handleResponse(int clientSocket, const shared_ptr<TCPData>& message);

        void sendLobby(shared_ptr<Client>& client, const shared_ptr<TCPData>& message);

        void leaveLobby(shared_ptr<Client>& client, const shared_ptr<TCPData>& message);

        void restoreClientState(const shared_ptr<Client>& client);

        void sendClientReconnectedFromAnotherLocation(int clientSocket);

        void handleLogin(int clientSocket, const shared_ptr<TCPData>& message);

};


#endif //UPSEM_MESSAGEHANDLER_H
