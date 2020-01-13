//
// Created by nvk on 01/02/2020.
//

#include "GameController.h"
#include "../Lobby.h"
#include "../../communication/handlers/GameMessageHandler.h"

GameController::GameController(Lobby& lobby) : lobby(lobby) {
    gameMessageHandler = make_shared<GameMessageHandler>(*this);
}

void GameController::cancelGame() {
    for (const auto& client : confirmedClients) {
        gameMessageHandler->sendGameCouldNotStart(client);
    }
    confirmedClients.clear();
    lobby.setLobbyState(LOBBY_STATE_WAITING);
    lobby.resetClientParticipation();
}

void GameController::startGame() {
    for (const auto& client : lobby.getClients()) {
        if (!this->containsConfirmedClient(client)) {
            lobby.removeClient(client);
            lobby.sendClientDidntConfirm(client);
        }
    }

    blackjack = make_unique<Blackjack>(confirmedClients);
    blackjack->dealCards();
    for (const auto& client : blackjack->getPlayers()) {
        gameMessageHandler->sendPrepareGameRequest(client);
    }
    this->sendBoardUpdate();

    gameMessageHandler->sendPlayerTurnRequest(blackjack->getCurrentPlayer());
    this->sendCurrentPlayer();
    lobby.setLobbyState(LOBBY_STATE_IN_GAME);
    lastGameUpdate = system_clock::now();
}

void GameController::sendBoardUpdate() {
    for (const auto& client : blackjack->getPlayers()) {
        gameMessageHandler->sendBoard(client, blackjack->getPlayers(), blackjack->getDealer());
    }
}

void GameController::sendCurrentPlayer() {
    auto currentPlayerUsername = blackjack->getCurrentPlayer()->getUsername();
    for (const auto& client : blackjack->getPlayers()) {
        gameMessageHandler->sendCurrentPlayer(client, currentPlayerUsername);
    }
}

void GameController::checkIfTimeForNextPlayer() {
    auto currentTime = system_clock::now();
    auto durationMillis = duration<double, milli>(currentTime - lastGameUpdate).count();

    if (durationMillis > MAX_TIME_BEFORE_STAND_MS || autoSkipPlayer(blackjack->getCurrentPlayer())) {
        blackjack->skipPlayer();
        if (blackjack->isGameRunning()) {
            gameMessageHandler->sendPlayerTurnRequest(blackjack->getCurrentPlayer());
            sendCurrentPlayer();
            sendBoardUpdate();
        }
    }
}

void GameController::handlePlayerTurn(const shared_ptr<Client>& client, const shared_ptr<TCPData>& message) {
    auto response = message->valueOf(TURN_TYPE);

    auto turnResult = make_shared<TurnResult>(RESULT_OK, nullptr, nullptr);
    if (response == STAND) {
        turnResult = blackjack->handleStand(client);
    } else if (response == HIT) {
        turnResult = blackjack->handleHit(client);
    } else if (response == DOUBLE_DOWN) {
        turnResult = blackjack->handleDoubleDown(client);
    }

    if (turnResult->getResult() == RESULT_NOT_YOUR_TURN) {
        gameMessageHandler->sendNotYourTurn(client);
    } else if (turnResult->getResult() == RESULT_DOUBLE_AFTER_HIT) {
        gameMessageHandler->sendDoubleDownAfterHit(client);
        gameMessageHandler->sendPlayerTurnRequest(client);
    } else {
        for (const auto& player : blackjack->getPlayers()) {
            gameMessageHandler->sendPlayerTurnResult(turnResult, player);
        }
        if (!isGameFinished()) {
            gameMessageHandler->sendPlayerTurnRequest(blackjack->getCurrentPlayer());
        }
        sendBoardUpdate();
    }
}

void GameController::prepareGame() {
    //Controller si zaznamena cas startu, klienti maji 60s na potvrzeni sazky
    preparationStart = system_clock::now();

    //Kazdemu klientovi je poslan dotaz s tim kolik vsadi
    for (const auto& client : lobby.getClients()) {
        gameMessageHandler->sendConfirmParticipationRequest(client);
    }
    lobby.setLobbyState(LOBBY_STATE_PREPARING);
}

void GameController::addConfirmedClient(shared_ptr<Client> client, int bet) {
    confirmedClients.push_back(client);
    client->createPlayerInfo(bet);
}

bool GameController::hasPreparationTimeExpired() {
    auto currentTime = system_clock::now();
    auto durationMillis = duration<double, milli>(currentTime - preparationStart).count();
    return durationMillis >= LOBBY_PREP_TIME_MS || lobby.getClients().size() == confirmedClients.size();
}

bool GameController::isPlayable() {
    return confirmedClients.size() >= 2;
}

bool GameController::isGameFinished() {
    return !blackjack->isGameRunning();
}

void GameController::endGame() {
    cout << "ending game" << endl;
    sendBoardUpdate();
    gameMessageHandler->sendResults(blackjack->getPlayers(), blackjack->getDealer());
    lobby.setLobbyState(LOBBY_STATE_FINISHED);
    lobby.prepareToReturn();
}

void GameController::removeData() {
    blackjack = nullptr;
    confirmedClients.clear();
    autoSkip.clear();
}

void GameController::reconnectClient(const shared_ptr<Client>& client) {
    auto message = TCPData(DATATYPE_RESPONSE);
    message.add(RESPONSE, LOGIN);
    message.add(RESTORE_STATE, GAME);
    gameMessageHandler->sendMessage(client->getClientSocket(), message.serialize());
    if (!isGameFinished()) {
        gameMessageHandler->sendBoard(client, blackjack->getPlayers(), blackjack->getDealer());
        if (blackjack->getCurrentPlayer() == client) {
            gameMessageHandler->sendPlayerTurnRequest(blackjack->getCurrentPlayer());
        }
        for (const auto& player : confirmedClients) {
            gameMessageHandler->sendShowPlayerReconnected(player, client->getUsername());
        }
    } else {
        gameMessageHandler->sendBoard(client, blackjack->getPlayers(), blackjack->getDealer());
        gameMessageHandler->sendResults(client, blackjack->getPlayers(), blackjack->getDealer());
    }
}

bool GameController::containsConfirmedClient(const shared_ptr<Client>& client) {
    for (const auto& player : confirmedClients) {
        if (player == client) {
            return true;
        }
    }
    return false;
}

void GameController::addAutoSkip(shared_ptr<Client> client) {
    autoSkip.push_back(client);
}

bool GameController::autoSkipPlayer(const shared_ptr<Client>& client) {
    for (const auto& player : autoSkip) {
        if (player == client) {
            return true;
        }
    }
    return false;
}