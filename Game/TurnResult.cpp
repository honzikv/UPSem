//
// Created by nvk on 12/30/2019.
//

#include "TurnResult.h"

TurnResult::TurnResult(const Result result, const shared_ptr<Card> card, const shared_ptr<Client> client)
        : result(result), card(card), client(client) {
}

const Result TurnResult::getResult() const {
    return result;
}

const shared_ptr<Card>& TurnResult::getCard() const {
    return card;
}

const shared_ptr<Client>& TurnResult::getClient() const {
    return client;
}
