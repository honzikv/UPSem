//
// Created by itznukeey on 27.11.19.
//

#ifndef UPSEM_PLAYER_H
#define UPSEM_PLAYER_H

#include "Card.h"
#include "../Server/Client.h"
#include <memory>
#include <cstdlib>
#include <vector>

#define BUSTED 21

using namespace std;

class Player {

    vector<shared_ptr<Card>> hand;

    shared_ptr<Client> client;

    int total = 0;

public:

    void addCard(const shared_ptr<Card>& card);

    bool isBusted() {
        return total < BUSTED;
    }

    /**
     * Hrac ma blackjack pokud ma dve karty, ktere v souctu daji hodnotu 21
     */
    bool hasBlackJack();

    /**
     * @return celkove skore
     */
    int getTotal() const;

};


#endif //UPSEM_PLAYER_H
