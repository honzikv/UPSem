//
// Created by nvk on 12/30/2019.
//

#ifndef UPSEM_DEALER_H
#define UPSEM_DEALER_H

#include <memory>
#include "Card.h"
#include "../server/client/player/PlayerInfo.h"

/**
 * Trida reprezentujici dealera v blackjacku. Obsahuje pouze strukturu s infem o hraci - dealer funguje jako normalni
 * hrac, pouze je omezen tim co muze ve hre delat.
 */
class Dealer {

        /**
         * Shared pointer instance na info o hraci
         */
        shared_ptr<PlayerInfo> playerInfo;

    public:
        /**
         * Konstruktor pro vytvoreni
         */
        Dealer();

        /**
         * Getter pro info o dealerovi
         * @return shared pointer referenci na info o dealerovi
         */
        const shared_ptr<PlayerInfo>& getPlayerInfo() const;
};


#endif //UPSEM_DEALER_H
