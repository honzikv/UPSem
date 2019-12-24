//
// Created by nvk on 12/24/2019.
//

#ifndef UPSEM_MESSAGEWRITER_H
#define UPSEM_MESSAGEWRITER_H

#include "Fields.h"
#include "Values.h"
#include "netinet/in.h"
#include "../serialization/TCPData.h"
#include "../Client.h"

class Server;

class MessageWriter {

        Server& server;
    public:
        explicit MessageWriter(Server& server);

    private:

        static bool sendMessage(int clientSocket, const string& message);

    public:
        static void sendUsernameUnique(int clientSocket, bool isUnique);

        void sendLobbyList(shared_ptr<Client>& client);

        void sendLobbyJoinable(shared_ptr<Client>& client, bool isJoinable, int lobbyId);

};


#endif //UPSEM_MESSAGEWRITER_H
