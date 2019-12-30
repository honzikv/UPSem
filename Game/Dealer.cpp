//
// Created by nvk on 12/30/2019.
//

#include "Dealer.h"

Dealer::Dealer() {
    playerInfo = make_shared<PlayerInfo>();
}

const shared_ptr<PlayerInfo>& Dealer::getPlayerInfo() const {
    return playerInfo;
}
