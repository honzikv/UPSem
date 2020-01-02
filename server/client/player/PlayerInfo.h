//
// Created by nvk on 12/29/2019.
//

#ifndef UPSEM_PLAYERINFO_H
#define UPSEM_PLAYERINFO_H

#include <memory>
#include <vector>
#include "../../../game/Card.h"
#include "../../communication/Constants.h"

using namespace std;

class PlayerInfo {

        vector<shared_ptr<Card>> hand;

        int bet;

        bool hit = false;

    public:

        explicit PlayerInfo(int bet);

        void addCard(shared_ptr<Card> card);

        void doHit();

        void doubleDown();

        bool hasHit();

        bool isBusted();

        const vector<shared_ptr<Card>>& getHand() const;

        int getSoftHandValue();

        bool hasBlackjack();

        bool handContainsRank(Card::Rank rank);

        int getHardHandValue();

        int getBet() const;

        int getTotal();
};


#endif //UPSEM_PLAYERINFO_H
