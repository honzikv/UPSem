#include "Server/Server.h"

int main() {

    auto server = make_shared<Server>(4422, 5);

    return EXIT_SUCCESS;
}