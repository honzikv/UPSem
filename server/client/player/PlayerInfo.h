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

/**
 * Trida obsahujici informace o hraci - jeho ruku, kolik vsadil atd.
 */
class PlayerInfo {

        /**
         * Ruka hrace
         */
        vector<shared_ptr<Card>> hand;

        /**
         * Sazka hrace
         */
        int bet;

        /**
         * Zda-li jiz hrac zahral HIT - aby nemohl zahrat double down po hitu
         */
        bool hit = false;

    public:

        /**
         * Konstruktor pro vytvoreni informacich o hraci - konstruktor se vyuziva pri kazde hre
         * @param bet kolik hrac vsadil
         */
        explicit PlayerInfo(int bet);

        /**
         * Prida do ruky dalsi kartu
         * @param card shared pointer karty
         */
        void addCard(shared_ptr<Card> card);

        /**
         * Nastavi flag hitu jako true
         */
        void doHit();

        /**
         * Provede double down - tzn. zdvojnasobi sazku
         */
        void doubleDown();

        /**
         * Getter flagu zda-li hrac zahral hit
         * @return boolean zda-li hrac zahral hit nebo ne
         */
        bool hasHit();

        /**
         * Vrati, zda-li je hrac busted - hodnota jeho ruky je pres 21
         * @return flag, zda-li je hrac busted
         */
        bool isBusted();

        /**
         * Pro ziskani ruky hrace
         * @return vrati referenci na shared pointer ruky hrace
         */
        const vector<shared_ptr<Card>>& getHand() const;

        /**
         * Vrati hodnotu ruky, kdy eso pocitame jako 11
         * @return hodnota ruky s hodnotou esa jako 11
         */
        int getSoftHandValue();

        /**
         * Vrati hodnotu ruky, kdy eso pocitame jako 1
         * @return hodnota ruky s hodnotou esa jako 1
         */
        int getHardHandValue();

        /**
         * Zjisti, zda-li ma hrac blackjack
         * @return zda-li ma hrac blackjack
         */
        bool hasBlackjack();

        /**
         * Vrati boolean zda-li obsahuje ruka specifickou hodnotu
         * @param rank hodnota karty
         * @return true pokud hodnotu obsahuje, jinak false
         */
        bool handContainsRank(Card::Rank rank);

        /**
         * Vrati sazku hrace
         * @return sazka hrace
         */
        int getBet() const;

        /**
         * Vrati celkovou hodnotu, ktera je neblize k hodnote 21 ale neprekracuje
         * @return celkovou hodnotu, ktera je blize k 21
         */
        int getTotal();
};


#endif //UPSEM_PLAYERINFO_H
