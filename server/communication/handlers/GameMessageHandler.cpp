//
// Created by nvk on 01/02/2020.
//

#include "GameMessageHandler.h"
#include "../../lobby/controller/GameController.h"

GameMessageHandler::GameMessageHandler(GameController& gameController) : gameController(gameController) {}

void GameMessageHandler::sendMessage(int clientSocket, const string& message) {
    send(clientSocket, message.c_str(), message.size(), 0);
}

void GameMessageHandler::sendGameCouldNotStart(const shared_ptr<Client>& client) {
    auto message = TCPData(DATATYPE_REQUEST);
    message.add(REQUEST, SHOW_GAME_START_FAILED);
    sendMessage(client->getClientSocket(), message.serialize());
}

void GameMessageHandler::sendPrepareGameRequest(const shared_ptr<Client>& client) {
    auto message = TCPData(DATATYPE_REQUEST);
    message.add(REQUEST, GAME);
    sendMessage(client->getClientSocket(), message.serialize());
}

void GameMessageHandler::sendBoard(const shared_ptr<Client>& client, const vector<shared_ptr<Client>>& players,
                                   const shared_ptr<Dealer>& dealer) {
    auto message = TCPData(DATATYPE_REQUEST);
    message.add(REQUEST, UPDATE_BOARD);

    auto playerNo = 0;
    for (const auto& player : players) {
        auto playerInfo = player->getPlayerInfo();
        message.add(PLAYER + to_string(playerNo), player->getUsername());
        message.add(PLAYER + to_string(playerNo) + TOTAL_VALUE,
                    to_string(playerInfo->getHardHandValue()) + ";" +
                    to_string(playerInfo->getSoftHandValue()));
        message.add(PLAYER + to_string(playerNo) + BET, to_string(playerInfo->getBet()));

        auto cardNo = 0;
        for (const auto& card : player->getPlayerInfo()->getHand()) {
            message.add(PLAYER + to_string(playerNo) + CARD + to_string(cardNo), card->toString());
            cardNo++;
        }
        playerNo++;
    }
    message.add(PLAYER_COUNT, to_string(players.size()));
    auto cardNo = 0;
    for (const auto& card : dealer->getPlayerInfo()->getHand()) {
        message.add(DEALER + string(CARD) + to_string(cardNo), card->toString());
        cardNo++;
    }

    sendMessage(client->getClientSocket(), message.serialize());
}

void GameMessageHandler::sendPlayerTurnRequest(const shared_ptr<Client>& client) {
    auto message = TCPData(DATATYPE_REQUEST);
    message.add(REQUEST, TURN);
    sendMessage(client->getClientSocket(), message.serialize());
}

void GameMessageHandler::sendCurrentPlayer(const shared_ptr<Client>& client, const string& username) {
    auto message = TCPData(DATATYPE_REQUEST);
    message.add(REQUEST, SHOW_CURRENT_PLAYER);
    message.add(USERNAME, username);
    sendMessage(client->getClientSocket(), message.serialize());
}

void GameMessageHandler::sendPlayerSkipped(const shared_ptr<Client>& client, const string& username) {
    auto message = TCPData(DATATYPE_REQUEST);
    message.add(REQUEST, SHOW_PLAYER_SKIPPED);
    message.add(USERNAME, username);
    sendMessage(client->getClientSocket(), message.serialize());
}

void GameMessageHandler::sendConfirmParticipationRequest(const shared_ptr<Client>& client) {
    auto message = TCPData(DATATYPE_REQUEST);
    message.add(REQUEST, CONFIRM_PARTICIPATION);
    sendMessage(client->getClientSocket(), message.serialize());
}

void GameMessageHandler::sendNotYourTurn(const shared_ptr<Client>& client) {
    auto message = TCPData(DATATYPE_RESPONSE);
    message.add(RESPONSE, NOT_YOUR_TURN);
    sendMessage(client->getClientSocket(), message.serialize());
}

void GameMessageHandler::sendDoubleDownAfterHit(const shared_ptr<Client>& client) {
    auto message = TCPData(DATATYPE_RESPONSE);
    message.add(RESPONSE, DOUBLE_AFTER_HIT);
    sendMessage(client->getClientSocket(), message.serialize());
}

void
GameMessageHandler::sendPlayerTurnResult(const shared_ptr<TurnResult>& turnResult, const shared_ptr<Client>& client) {
    auto message = TCPData(DATATYPE_REQUEST);
    message.add(REQUEST, SHOW_PLAYER_TURN);
    message.add(USERNAME, turnResult->getClient()->getUsername());
    if (turnResult->getCard() == nullptr) {
        message.add(TURN_TYPE, STAND);
    } else {
        if (turnResult->getResult() == RESULT_DOUBLE_BUSTED) {
            message.add(TURN_TYPE, DOUBLE_DOWN);
            message.add(RESULT, BUSTED);
        } else if (turnResult->getResult() == RESULT_DOUBLE_OK) {
            message.add(TURN_TYPE, DOUBLE_DOWN);
            message.add(RESULT, OK);
        } else if (turnResult->getResult() == RESULT_BUSTED) {
            message.add(TURN_TYPE, HIT);
            message.add(RESULT, BUSTED);
        } else if (turnResult->getResult() == RESULT_OK) {
            message.add(TURN_TYPE, HIT);
            message.add(RESULT, OK);
        }
        message.add(CARD, turnResult->getCard()->toString());
    }

    sendMessage(client->getClientSocket(), message.serialize());
}

string GameMessageHandler::getSerializedGameResult(const vector<shared_ptr<Client>>& clients,
                                                   const shared_ptr<Dealer>& dealer) {
    auto message = TCPData(DATATYPE_REQUEST);
    message.add(REQUEST, SHOW_RESULTS);

    auto dealerInfo = dealer->getPlayerInfo();
    message.add(TOTAL_VALUE,
                to_string(dealerInfo->getHardHandValue()) + ";" + to_string(dealerInfo->getSoftHandValue()));
    auto playerNo = 0;

    for (const auto& player : clients) {
        auto playerInfo = player->getPlayerInfo();
        auto amountBet = playerInfo->getBet();
        auto amountWon = 0;

        //Pokud prohral hrac, nic nedostane bez ohledu na to, jak dopadl dealer
        if (playerInfo->isBusted()) {
            message.add(PLAYER + to_string(playerNo), LOSS);
        }
            //Pokud hrac neprekrocil 21 a dealer ano, vyhraje pokazde
        else if (dealerInfo->isBusted()) {
            message.add(PLAYER + to_string(playerNo), WIN);
            amountWon = (playerInfo->hasBlackjack() ? 2.5 : 2) * amountBet;
        } else {
            //Jinak, pokud jak hrac, tak dealer maji blackjack, hrac dostane svoji sazku zpet
            if (playerInfo->hasBlackjack() && dealerInfo->hasBlackjack()) {
                message.add(PLAYER + to_string(playerNo), TIE);
                amountWon = amountBet;
            }
                //Pokud ma hrac blackjack a dealer ne dostane 2.5 nasobek
            else if (playerInfo->hasBlackjack()) {
                amountWon = 2.5 * amountBet;
                message.add(PLAYER + to_string(playerNo), WIN);
            }
                //Pokud ma hrac vice nez dealer, vyhraje standardni castku
            else if (playerInfo->getTotal() > dealerInfo->getTotal()) {
                message.add(PLAYER + to_string(playerNo), WIN);
                amountWon = 2 * amountBet;
            }
                //Jinak hrac prohral
            else {
                message.add(PLAYER + to_string(playerNo), LOSS);
            }
        }

        message.add(PLAYER + to_string(playerNo) + BET, to_string(amountBet) + ";" + to_string((int) amountWon));
        playerNo++;
    }
    return message.serialize();
}

void GameMessageHandler::sendResults(const vector<shared_ptr<Client>>& clients, const shared_ptr<Dealer>& dealer) {
    auto serializedMessage = getSerializedGameResult(clients, dealer);
    for (const auto& player : clients) {
        sendMessage(player->getClientSocket(), serializedMessage);
        sendGameFinished(player);
    }
}

void GameMessageHandler::sendResults(const shared_ptr<Client>& client, const vector<shared_ptr<Client>>& clients,
                                     const shared_ptr<Dealer>& dealer) {
    auto serializedMessage = getSerializedGameResult(clients, dealer);
    sendMessage(client->getClientSocket(), serializedMessage);
}

void GameMessageHandler::sendGameFinished(const shared_ptr<Client>& client) {
    auto message = TCPData(DATATYPE_REQUEST);
    message.add(REQUEST, SHOW_RETURN_TO_LOBBY);
    auto timeBeforeReturnSec = (int) (TIME_BEFORE_RETURN_TO_LOBBY / 1e3);
    message.add(TIME, to_string(timeBeforeReturnSec));
    sendMessage(client->getClientSocket(), message.serialize());
}

void GameMessageHandler::sendShowPlayerReconnected(const shared_ptr<Client>& client, const string& username) {
    auto message = TCPData(DATATYPE_REQUEST);
    message.add(REQUEST, SHOW_PLAYER_RECONNECTED);
    message.add(USERNAME, username);
    sendMessage(client->getClientSocket(), message.serialize());
}