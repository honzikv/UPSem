#include "Server/Server.h"

int main() {

//    auto server = make_shared<Server>(4431, 5);
//
//    return EXIT_SUCCESS;

    auto deck = Deck();

    for (auto i = 0; i < 52; i++) {
        auto card = deck.getTop();
        cout << card->toString() << " " << card->getValue() << endl;
    }
    cout << deck.cardsLeft() << endl;
}