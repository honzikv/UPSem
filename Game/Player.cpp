//
// Created by itznukeey on 27.11.19.
//

#include "Player.h"

void Player::addCard(const shared_ptr<Card>& card) {
    hand.push_back(card);
    total += card.get()->getValue();
}

int Player::getTotal() const {
    return total;
}

bool Player::hasBlackJack() {
    return total == 21 && hand.size() == 2;
}
