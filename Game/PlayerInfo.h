//
// Created by nvk on 12/29/2019.
//

#ifndef UPSEM_PLAYERINFO_H
#define UPSEM_PLAYERINFO_H

#include <memory>
#include <vector>
#include "Card.h"
#include "../Server/communication/Constants.h"

using namespace std;

class PlayerInfo {

    vector<shared_ptr<Card>> hand;

    bool busted = false;

    bool playing = true;

    bool cardsReceived = false;

    public:
        void addCard(shared_ptr<Card> card);

        bool isBusted();

        const vector<shared_ptr<Card>>& getHand() const;

        void confirmCardsReceived();

        int getHandValue();

        bool isPlaying() const;

        void setFinishedPlaying();
};


#endif //UPSEM_PLAYERINFO_H
