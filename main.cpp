

#include <iostream>
#include "json-parsing/JSONParser.h"
#include "Server/Server.h"

using json = nlohmann::json;

int main() {
    auto server = Server(8080,12);
    server.run();
}