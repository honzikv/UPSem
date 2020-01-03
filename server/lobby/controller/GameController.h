//
// Created by nvk on 01/02/2020.
//

#ifndef UPSEM_GAMECONTROLLER_H
#define UPSEM_GAMECONTROLLER_H


#include "../../../game/Blackjack.h"
#include "../../communication/serialization/TCPData.h"

class Lobby;

class GameMessageHandler;

class GameController {

        Lobby& lobby;

        unique_ptr<Blackjack> blackjack;

        vector<shared_ptr<Client>> confirmedClients;

        shared_ptr<GameMessageHandler> gameMessageHandler;

        chrono::time_point<chrono::system_clock> preparationStart;

        chrono::time_point<chrono::system_clock> lastGameUpdate;

    public:
        explicit GameController(Lobby& lobby);

        void prepareGame();

        void addConfirmedClient(shared_ptr<Client> client, int bet);

        void reconnectClient(const shared_ptr<Client>& client);

        bool isPlayable();

        void cancelGame();

        void startGame();

        void sendBoardUpdate();

        void checkIfTimeForNextPlayer();

        void sendCurrentPlayer();

        void handlePlayerTurn(const shared_ptr<Client>& player, const shared_ptr<TCPData>& message);

        void endGame();

        bool isGameFinished();

        bool hasPreparationTimeExpired();

        bool containsConfirmedClient(const shared_ptr<Client>& client);

        void removeData();
};


#endif //UPSEM_GAMECONTROLLER_H
