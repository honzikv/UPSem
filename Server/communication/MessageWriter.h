//
// Created by nvk on 12/23/2019.
//

#ifndef UPSEM_MESSAGEWRITER_H
#define UPSEM_MESSAGEWRITER_H

#include "Fields.h"
#include "Values.h"
#include "../serialization/DataType.h"
#include "../serialization/TCPData.h"
#include "../Client.h"
#include "../Server.h"

class MessageWriter {

        Server& server;

        static bool sendMessage(int clientSocket, const string& message);

    public:
        static void sendUsernameUnique(int clientSocket, bool isUnique);

        void sendLobbyList(Client& client);

        void sendLobbyJoinable(Client& client, TCPData& data);
};


#endif //UPSEM_MESSAGEWRITER_H
