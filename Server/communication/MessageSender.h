//
// Created by nvk on 12/07/2019.
//

#ifndef UPSEM_MESSAGESENDER_H
#define UPSEM_MESSAGESENDER_H

#include "../Server.h"
#include "../serialization/Serializer.h"


class MessageSender {

private:
    /**
     * Filedescriptor pripojeni
     */
    int fileDescriptor;

    unique_ptr<Serializer> serializer;

    Server& server;

    bool sendToClient(const string& data) {
        return send(fileDescriptor, data.c_str(), data.length(), 0) < 0;
    }

public:
    MessageSender(int fileDescriptor, Server& server) : fileDescriptor(fileDescriptor), server(server) {
        this->serializer = make_unique<Serializer>();
    }

    void sendLoginUnique(bool isUnique) {
        serializer->clear();
        serializer->append("response", to_string(isUnique));
        if (!sendToClient(serializer->serialize())) {
            throw exception();
        }
    }

    void sendLobbyNotJoinable() {
        serializer->clear();
        serializer->append("response", to_string(false));
        if (!sendToClient((serializer->serialize()))) {
            throw exception();
        }
    }

    void sendLobbies() {
        serializer->clear();

        for (const auto& lobby : server.getLobbies()) {
            serializer->append(to_string(lobby->getId()), lobby->getState());
        }

        if (!sendToClient(serializer->serialize())) {
            throw exception();
        }
    }
};


#endif //UPSEM_MESSAGESENDER_H
