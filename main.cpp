

#include <iostream>
#include "json-parsing/JSONParser.h"
#include "Server/Server.h"

using JSON = nlohmann::json;

#include "Game/Blackjack.h"


int main() {
//    auto server = Server(8080,12);
//    server.run();

    auto blackJack = make_shared<Blackjack>(12);
    auto player = make_shared<Player>();
    auto deck = make_shared<Deck>();
    deck.get()->shuffle();

    for (auto i = 0; i < 5; i++) {
        player.get()->addCard(deck.get()->getTop());
    }

    cout << player.get()->getTotal();

}