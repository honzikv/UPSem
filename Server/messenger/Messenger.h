//
// Created by nvk on 12/05/2019.
//

#ifndef UPSEM_MESSENGER_H
#define UPSEM_MESSENGER_H

#include "../serialization/Serializer.h"
#include "../serialization/Deserializer.h"
#include "../Client.h"
#include "../Lobby.h"

class Messenger {

    unique_ptr<Serializer> serializer;

    unique_ptr<Deserializer> deserializer;

public:
    Messenger() {
        serializer = make_unique<Serializer>();
        deserializer = make_unique<Deserializer>();
    }

public:
    bool sendAuthRequest(Client& client) {
        serializer->clear();
        serializer->append("request", "login");
        auto message = serializer->serialize();
        return send(client.getFileDescriptor(), message.c_str(), message.length(), 0) > 0;
    }

    bool sendLobbies(Client& client, const vector<Lobby>& lobbies) {
        serializer->clear();
        serializer->append("count", to_string(lobbies.size()));

        for (auto lobby : lobbies) {
            serializer->append("lobby" + to_string(lobby.getId()), lobby.getState());
        }

        auto message = serializer->serialize();
        return send(client.getFileDescriptor(), message.c_str(), message.length(), 0) > 0;
    }
};


#endif //UPSEM_MESSENGER_H
