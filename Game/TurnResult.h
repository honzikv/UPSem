//
// Created by nvk on 12/30/2019.
//

#ifndef UPSEM_TURNRESULT_H
#define UPSEM_TURNRESULT_H

#include <memory>
#include "Result.h"
#include "Card.h"
#include "../Server/Client.h"

using namespace std;

class TurnResult {

     const Result result;

     const shared_ptr<Card> card;

     const shared_ptr<Client> client;

    public:
        TurnResult(const Result result, const shared_ptr<Card> card, const shared_ptr<Client> client);

    public:
        const Result getResult() const;

        const shared_ptr<Card>& getCard() const;

        const shared_ptr<Client>& getClient() const;
};


#endif //UPSEM_TURNRESULT_H
