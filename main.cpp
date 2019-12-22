

#include <iostream>
#include "Server/Server.h"
#include "Game/Blackjack.h"



int main() {

    auto server = Server(4431,10);
    server.run();
}