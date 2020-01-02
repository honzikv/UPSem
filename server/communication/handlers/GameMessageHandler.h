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

        void sendMessage(int clientSocket, const string& message);

    public:
        explicit GameMessageHandler(GameController& gameController);

        void sendConfirmParticipationRequest(const shared_ptr<Client>& client);

        void sendGameCouldNotStart(const shared_ptr<Client>& client);

        void sendBoard(const shared_ptr<Client>& player, const vector<shared_ptr<Client>>& players,
                       const shared_ptr<Dealer>& dealer);

        void sendPlayerTurnRequest(const shared_ptr<Client>& client);

        void sendCurrentPlayer(const shared_ptr<Client>& client, const string& username);

        void sendNotYourTurn(const shared_ptr<Client>& client);

        void sendPlayerTurnResult(const shared_ptr<TurnResult>& turnResult, const shared_ptr<Client>& client);

        void sendDoubleDownAfterHit(const shared_ptr<Client>& client);

        void sendResults(const vector<shared_ptr<Client>>& clients, const shared_ptr<Dealer>& dealer);

        void sendShowMessage(const shared_ptr<Client>& client, const string& message);

};


#endif //UPSEM_GAMEMESSAGEHANDLER_H
