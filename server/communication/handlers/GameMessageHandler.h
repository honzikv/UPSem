//
// Created by nvk on 01/02/2020.
//

#ifndef UPSEM_GAMEMESSAGEHANDLER_H
#define UPSEM_GAMEMESSAGEHANDLER_H

#include <string>
#include <netinet/in.h>
#include "../serialization/TCPData.h"
#include "../../communication/Fields.h"
#include "../../communication/Values.h"
#include "../../client/Client.h"
#include "../../../game/Dealer.h"
#include "../../../game/TurnResult.h"

using namespace std;

class GameController;

class GameMessageHandler {

        GameController& gameController;

    public:
        explicit GameMessageHandler(GameController& gameController);

        static void sendConfirmParticipationRequest(const shared_ptr<Client>& client);

        static void sendGameCouldNotStart(const shared_ptr<Client>& client);

        static void sendPrepareGameRequest(const shared_ptr<Client>& client);

        static void sendBoard(const shared_ptr<Client>& player, const vector<shared_ptr<Client>>& players,
                       const shared_ptr<Dealer>& dealer);

        static void sendPlayerTurnRequest(const shared_ptr<Client>& client);

        static void sendCurrentPlayer(const shared_ptr<Client>& client, const string& username);

        static void sendNotYourTurn(const shared_ptr<Client>& client);

        static void sendPlayerTurnResult(const shared_ptr<TurnResult>& turnResult, const shared_ptr<Client>& client);

        static void sendDoubleDownAfterHit(const shared_ptr<Client>& client);

        static void sendResults(const vector<shared_ptr<Client>>& clients, const shared_ptr<Dealer>& dealer);

        static void sendGameFinished(const shared_ptr<Client>& client);

        static void sendShowPlayerReconnected(const shared_ptr<Client>& client, const string& username);

        static string getSerializedGameResult(const vector<shared_ptr<Client>>& clients, const shared_ptr<Dealer>& dealer);

        void sendResults(const shared_ptr<Client>& client, const vector<shared_ptr<Client>>& clients,
                         const shared_ptr<Dealer>& dealer);

        static void sendMessage(int clientSocket, const string& message);
};


#endif //UPSEM_GAMEMESSAGEHANDLER_H
