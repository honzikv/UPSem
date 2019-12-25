#include "Server/Server.h"

int main() {

    auto server = Server(44321, 10);
    server.run();
}