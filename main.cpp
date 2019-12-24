#include "Server/Server.h"

int main() {

    auto server = Server(4431, 10);
    server.run();
}