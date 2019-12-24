//
// Created by nvk on 12/24/2019.
//

#ifndef UPSEM_MESSAGEHANDLER_H
#define UPSEM_MESSAGEHANDLER_H

class Server;

class MessageHandler {

        Server& server;

    public:
        MessageHandler(Server& server);
};


#endif //UPSEM_MESSAGEHANDLER_H
