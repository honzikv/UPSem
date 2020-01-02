//
// Created by nvk on 12/29/2019.
//

#include "PlayerInfo.h"

int PlayerInfo::getSoftHandValue() {
    auto value = 0;
    for (const auto& card : hand) {
        if (card->getRank() == Card::ACE) {
            value += ACE_SOFT_VALUE;
        } else {
            value += card->getValue();
        }
    }
    return value;
}

int PlayerInfo::getHardHandValue() {
    auto value = 0;
    for (const auto& card : hand) {
        value += card->getValue();
    }
    return value;
}

bool PlayerInfo::hasBlackjack() {
    return hand.size() == 2 && handContainsRank(Card::ACE) ? handContainsRank(Card::TEN) ||
                                                             handContainsRank(Card::JACK) ||
                                                             handContainsRank(Card::QUEEN) ||
                                                             handContainsRank(Card::KING) : false;
}

bool PlayerInfo::isBusted() {
    auto totalValue = getHardHandValue();
    return totalValue > BLACKJACK;
}

int PlayerInfo::getTotal() {
    if (getSoftHandValue() > BLACKJACK) {
        return getHardHandValue();
    }
    return getSoftHandValue();
}

void PlayerInfo::addCard(shared_ptr<Card> card) {
    hand.push_back(card);
}

const vector<shared_ptr<Card>>& PlayerInfo::getHand() const {
    return hand;
}

bool PlayerInfo::handContainsRank(Card::Rank rank) {
    for (const auto& card : hand) {
        if (card->getRank() == rank) {
            return true;
        }
    }
    return false;
}

void PlayerInfo::doHit() {
    hit = true;
}

bool PlayerInfo::hasHit() {
    return hit;
}

PlayerInfo::PlayerInfo(int bet) : bet(bet) {
}

void PlayerInfo::doubleDown() {
    bet *= 2;
}

int PlayerInfo::getBet() const {
    return bet;
}
