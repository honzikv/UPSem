//
// Created by nvk on 12/29/2019.
//

#include "PlayerInfo.h"

int PlayerInfo::getHandValue() {
    auto value = 0;
    for (const auto& card : hand) {
        value += card->getValue();
    }
    return value;
}

bool PlayerInfo::isBusted() {
    auto totalValue = getHandValue();
    return totalValue >= BLACKJACK;
}

void PlayerInfo::addCard(shared_ptr<Card> card) {
    hand.push_back(card);
}

const vector<shared_ptr<Card>>& PlayerInfo::getHand() const {
    return hand;
}


void PlayerInfo::setFinishedPlaying() {
    playing = false;
}

bool PlayerInfo::isPlaying() const {
    return playing;
}

