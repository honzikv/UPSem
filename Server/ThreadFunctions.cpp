//
// Created by itznukeey on 21.11.19.
//

#include <cstring>
#include <sys/socket.h>
#include "Server.h"

static shared_ptr<Client> handleLogin(int clientSocket, Server& server) {

    int bytesRead;
    auto timeStart = clock();

    do {
        auto loginMessage = "Enter username";
        char messageBuffer[1024] = {0};
        send(clientSocket, loginMessage, strlen(loginMessage), 0);
        bytesRead = recv(clientSocket, messageBuffer, MESSAGE_BUFFER_BYTES, 0);

        //zpravu ulozi do stringu
        auto response = string(messageBuffer);
        auto jsonParser = server.getJsonParser();
        //zkusi precist odpoved, pokud se nejedna o nesmysl pokracuje
        auto responseValue = jsonParser.parseStringValue(LOGIN_FIELD, response);

        if (!response.empty() && server.isLoginUnique(responseValue)) {
            auto client = make_shared<Client>(responseValue, clientSocket);
            server.addClient(client);
        }

    } while (bytesRead >= 0 && ((double(clock() - timeStart) / CLOCKS_PER_SEC <= DEFAULT_LOGIN_TIMEOUT_SECONDS)));

    return nullptr;
}