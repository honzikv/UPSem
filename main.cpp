#include "Server/Server.h"

int main() {

    auto server = make_shared<Server>(44312, 10);
    server->run();
}