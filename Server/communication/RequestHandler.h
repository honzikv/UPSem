//
// Created by nvk on 12/23/2019.
//

#ifndef UPSEM_REQUESTHANDLER_H
#define UPSEM_REQUESTHANDLER_H

#include "../Server.h"
#include "Values.h"
#include "MessageWriter.h"

class RequestHandler {

        Server& server;

        MessageWriter& messageWriter;

    public:
        void handleSocketRequest(TCPData& message, int& clientSocket);

        void handleClientRequest(TCPData& message, shared_ptr<Client>& client);

};


#endif //UPSEM_REQUESTHANDLER_H
