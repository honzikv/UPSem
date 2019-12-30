//
// Created by nvk on 12/30/2019.
//

#ifndef UPSEM_DEALER_H
#define UPSEM_DEALER_H

#include <memory>
#include "Card.h"
#include "PlayerInfo.h"

class Dealer {

        shared_ptr<PlayerInfo> playerInfo;

    public:
        Dealer();

        const shared_ptr<PlayerInfo>& getPlayerInfo() const;
};


#endif //UPSEM_DEALER_H
