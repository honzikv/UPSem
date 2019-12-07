//
// Created by nvk on 12/07/2019.
//

#ifndef UPSEM_MESSAGERECEIVER_H
#define UPSEM_MESSAGERECEIVER_H

#include "../serialization/Deserializer.h"
#include "../Server.h"
#include "Constants.h"


class MessageReceiver {

private:
    int fileDescriptor;

    Server& server;

    unique_ptr<Deserializer> deserializer;

public:
    MessageReceiver(int fileDescriptor, Server& server);

    string getUsername() {
        char buffer[MAX_BUFFER_SIZE_BYTES];
        memset(buffer, 0, MAX_BUFFER_SIZE_BYTES);

        if (read(fileDescriptor, buffer, MAX_BUFFER_SIZE_BYTES) < 0) {
            throw exception();
        }

        deserializer->deserialize(buffer);
        auto username = deserializer->valueOf("login");
        return server.isLoginUnique(username) ? username : NULL;
    }

    int getSelectedLobby() {
        char buffer[MAX_BUFFER_SIZE_BYTES];
        memset(buffer, 0, MAX_BUFFER_SIZE_BYTES);

        if (read(fileDescriptor, buffer, MAX_BUFFER_SIZE_BYTES) < 0) {
            throw exception();
        }

        deserializer->deserialize(buffer);
        auto lobbyIdString = deserializer->valueOf("lobbyId");
        auto lobbyId = stoi(lobbyIdString);

        return lobbyId >= server.getLobbies().size() || server.getLobbies().at(lobbyId)->isFull() ? INT32_MIN : lobbyId;
    }
};


#endif //UPSEM_MESSAGERECEIVER_H
