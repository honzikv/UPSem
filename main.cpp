#include "server/Server.h"

#define DEFAULT_PORT 4431
#define DEFAULT_LOBBY_COUNT 3
#define MAX_PORT 10000
#define MAX_LOBBY_COUNT 10
#define LOCALHOST "127.0.0.1"

bool isNumber(const string& numberString) {
    return !numberString.empty() && find_if(numberString.begin(),
                                            numberString.end(),
                                            [](unsigned char c) {
                                                return !isdigit(c);
                                            }) == numberString.end();
}

int parsePort(const string& portString) {
    if (!isNumber(portString)) {
        return -1;
    } else {
        auto port = stoi(portString);
        return port < 1 || port > MAX_PORT ? -1 : port;
    }
}

int parseLobbyCount(const string& lobbyCountString) {
    if (!isNumber(lobbyCountString)) {
        return -1;
    } else {
        auto lobbyCount = stoi(lobbyCountString);
        return lobbyCount < 1 || lobbyCount > MAX_LOBBY_COUNT ? -1 : lobbyCount;
    }
}

bool isIPParseable(const string& ipString) {
    unsigned char buf[sizeof(struct in6_addr)];
    return inet_pton(AF_INET, ipString.c_str(), buf) == 1;
}

void checkErrors(int port, int lobbyCount) {
    if (port == -1) {
        cerr << "Entered port is invalid, it must be an integer between 1 and " << MAX_PORT << endl;
        exit(EXIT_FAILURE);
    }
    if (lobbyCount == -1) {
        cerr << "Entered number of lobbies is invalid, it must be an integer between 1 and " << MAX_LOBBY_COUNT
             << endl;
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char** args) {
    if (argc == 1) {
        auto server = make_shared<Server>(LOCALHOST, DEFAULT_PORT, DEFAULT_LOBBY_COUNT);
    } else if (argc == 2) {
        auto port = parsePort(args[1]);
        checkErrors(port, DEFAULT_LOBBY_COUNT);
        auto server = make_shared<Server>(LOCALHOST, port, DEFAULT_LOBBY_COUNT);
    } else if (argc == 3) {
        auto port = parsePort(args[1]);
        auto lobbyCount = parseLobbyCount(args[2]);
        checkErrors(port, lobbyCount);
        auto server = make_shared<Server>(LOCALHOST, port, lobbyCount);
    } else if (argc >= 3) {
        auto port = parsePort(args[2]);
        auto lobbyCount = parseLobbyCount(args[3]);

        if (!isIPParseable(args[1])) {
            cerr << "Error address is not parseable << please use ipv4 format" << endl;
            exit(EXIT_FAILURE);
        }
        checkErrors(port, lobbyCount);
        auto server = Server(args[1], port, lobbyCount);
    }
}
