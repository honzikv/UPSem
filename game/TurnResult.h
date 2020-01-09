//
// Created by nvk on 12/30/2019.
//

#ifndef UPSEM_TURNRESULT_H
#define UPSEM_TURNRESULT_H

#include <memory>
#include "Result.h"
#include "Card.h"
#include "../server/client/Client.h"

using namespace std;

/**
 * Objekt pro prenos vysledku tahu hrace
 */
class TurnResult {

        /**
         * Vysledek tahu
         */
        const Result result;

        /**
         * Ziskana karta (muze byt nullptr)
         */
        const shared_ptr<Card> card;

        /**
         * Hrac, ktery kolo zahral
         */
        const shared_ptr<Client> client;

    public:
        /**
         * Konstruktor pro vytvoreni DTO
         * @param result vysledek tahu
         * @param card tazena karta (optional)
         * @param client hrac, ktery kolo zahral
         */
        TurnResult(Result result, shared_ptr<Card> card, shared_ptr<Client> client);

    public:
        /**
         * Getter pro enum result
         * @return vrati vysledek
         */
        const Result getResult() const;

        /**
         * Getter pro tazenou kartu
         * @return vrati tazenou kartu
         */
        const shared_ptr<Card>& getCard() const;

        /**
         * Getter pro hrace
         * @return vrati referenci na hrace
         */
        const shared_ptr<Client>& getClient() const;
};


#endif //UPSEM_TURNRESULT_H
