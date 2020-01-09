//
// Created by nvk on 01/02/2020.
//

#ifndef UPSEM_GAMECONTROLLER_H
#define UPSEM_GAMECONTROLLER_H


#include "../../../game/Blackjack.h"
#include "../../communication/serialization/TCPData.h"

using namespace std::chrono;

class Lobby;

class GameMessageHandler;

/**
 * Trida, ktera slouzi k ovladani hry, je soucasti lobby a nemela by se vytvaret jinde nez v lobby
 */
class GameController {

        /**
         * Reference na lobby
         */
        Lobby& lobby;

        /**
         * Pointer na blackjack ma ulozeny pouze v sobe a nikomu ho nepredava
         */
        unique_ptr<Blackjack> blackjack;

        /**
         * Seznam potvrzenych klientu
         */
        vector<shared_ptr<Client>> confirmedClients;

        /**
         * Seznam klientu, pro ktere udela autoskip - takovi klienti, kteri ve hre dali Leave, protoze si hrani rozmysleli
         */
        vector<shared_ptr<Client>> autoSkip;

        /**
         * GameMessageHandler pro zasilani dotazu a odpovedi klientum
         */
        shared_ptr<GameMessageHandler> gameMessageHandler;

        /**
         * Cas pripravy hry
         */
        time_point<system_clock> preparationStart;

        /**
         * Posledni update hry
         */
        time_point<system_clock> lastGameUpdate;

    public:
        explicit GameController(Lobby& lobby);

        void prepareGame();

        void addConfirmedClient(shared_ptr<Client> client, int bet);

        void reconnectClient(const shared_ptr<Client>& client);

        bool isPlayable();

        void cancelGame();

        void startGame();

        void sendBoardUpdate();

        void addAutoSkip(shared_ptr<Client> client);

        void checkIfTimeForNextPlayer();

        void sendCurrentPlayer();

        void handlePlayerTurn(const shared_ptr<Client>& player, const shared_ptr<TCPData>& message);

        void endGame();

        bool isGameFinished();

        bool hasPreparationTimeExpired();

        bool containsConfirmedClient(const shared_ptr<Client>& client);

        void removeData();

        bool autoSkipPlayer(const shared_ptr<Client>& client);
};


#endif //UPSEM_GAMECONTROLLER_H
