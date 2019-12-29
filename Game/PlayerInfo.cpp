//
// Created by nvk on 12/29/2019.
//

#include "PlayerInfo.h"

bool PlayerInfo::isBusted() {
    auto totalValue = 0;
    for (const auto& card : hand) {
        totalValue += card->getValue();
    }
    return totalValue >= BLACKJACK;
}

void PlayerInfo::addCard(shared_ptr<Card> card) {
    hand.push_back(card);
    cardsReceived = false;
}

const vector<shared_ptr<Card>>& PlayerInfo::getHand() const {
    return hand;
}

void PlayerInfo::confirmCardsReceived() {
    cardsReceived = true;
}

void PlayerInfo::setFinishedPlaying() {
    playing = false;
}
