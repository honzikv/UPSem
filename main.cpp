

#include <iostream>
#include "Server/Server.h"
#include "Game/Blackjack.h"
#include "Server/serialization/Deserializer.h"
#include "Server/serialization/Serializer.h"


int main() {

    auto serializer = Serializer();
    serializer.clear();
    serializer.append("test", "ok");
    serializer.append("test23", "boomer");
    cout << serializer.serialize() << endl;

}