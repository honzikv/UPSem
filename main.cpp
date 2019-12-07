

#include <iostream>
#include "Server/Server.h"
#include "Game/Blackjack.h"
#include "Server/serialization/Deserializer.h"
#include "Server/serialization/Serializer.h"



int main() {

    auto server = Server(4431,10);
    server.run();
}