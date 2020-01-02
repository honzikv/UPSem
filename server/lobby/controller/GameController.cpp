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
        if (!this->contains(client)) {
            lobby.removeClient(client);
            lobby.sendClientDidntConfirm(client);
        }
    }

    blackjack = make_unique<Blackjack>(confirmedClients);
    blackjack->dealCards();
    sendBoardUpdate();

    gameMessageHandler->sendPlayerTurnRequest(blackjack->getCurrentPlayer());
    sendCurrentPlayer();
    lobby.setLobbyState(LOBBY_STATE_IN_GAME);
    lobby.resetClientParticipation();
    lastGameUpdate = chrono::system_clock::now();
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
    auto currentTime = chrono::system_clock::now();
    auto durationMillis = chrono::duration<double, milli>(currentTime - lastGameUpdate).count();

    if (durationMillis > MAX_TIME_BEFORE_STAND_MS) {
        blackjack->skipPlayer();
        blackjack->moveToNextPlayer();
        gameMessageHandler->sendPlayerTurnRequest(blackjack->getCurrentPlayer());
        sendCurrentPlayer();
        sendBoardUpdate();
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
    } else if (turnResult->getResult() == RESULT_HIT_AFTER_DOUBLE) {
        gameMessageHandler->sendDoubleDownAfterHit(client);
    } else {
        gameMessageHandler->sendPlayerTurnResult(turnResult, client);
        gameMessageHandler->sendPlayerTurnRequest(blackjack->getCurrentPlayer());
        sendBoardUpdate();
    }
}

void GameController::prepareGame() {
    preparationStart = chrono::system_clock::now();

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
    auto currentTime = chrono::system_clock::now();
    auto durationMillis = chrono::duration<double, milli>(currentTime - preparationStart).count();
    return durationMillis >= LOBBY_PREP_TIME_MS || lobby.getClients().size() == confirmedClients.size();
}

bool GameController::isPlayable() {
    return confirmedClients.size() >= 2;
}

bool GameController::hasGameFinished() {
    return !blackjack->isGameRunning();
}

void GameController::endGame() {
    sendBoardUpdate();
    gameMessageHandler->sendResults(blackjack->getPlayers(), blackjack->getDealer());


    blackjack = nullptr;
    confirmedClients.clear();
    lobby.setLobbyState(LOBBY_STATE_FINISHED);
    lobby.prepareToReturn();
}

void GameController::reconnectClient(const shared_ptr<Client>& client) {
    gameMessageHandler->sendBoard(client, blackjack->getPlayers(), blackjack->getDealer());
    if (blackjack->getCurrentPlayer() == client) {
        gameMessageHandler->sendPlayerTurnRequest(blackjack->getCurrentPlayer());
    }

    auto message = "Player " + client->getUsername() + " has reconnected.";
    for (const auto& player : confirmedClients) {
        gameMessageHandler->sendShowMessage(player, message);
    }
}

bool GameController::contains(const shared_ptr<Client>& client) {
    for (const auto& player : confirmedClients) {
        if (player == client) {
            return true;
        }
    }
    return false;
}


