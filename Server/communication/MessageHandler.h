//
// Created by nvk on 12/24/2019.
//

#ifndef UPSEM_MESSAGEHANDLER_H
#define UPSEM_MESSAGEHANDLER_H

#include "MessageWriter.h"

class Server;

class MessageHandler {

        Server& server;

        unique_ptr<MessageWriter> messageWriter;

    public:
        explicit MessageHandler(Server& server);

        void handleSocketMessage(int clientSocket, shared_ptr<TCPData>& message);

        void handleClientMessage(shared_ptr<Client>& client, shared_ptr<TCPData>& message);

        void handleRequest(shared_ptr<Client>& client, shared_ptr<TCPData>& message);

        void handleResponse(shared_ptr<Client>& sharedPtr, shared_ptr<TCPData>& message);

        void pingBack(shared_ptr<Client>& sharedPtr);
};


#endif //UPSEM_MESSAGEHANDLER_H
