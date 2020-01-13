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

/**
 * Trida s metodami pro zasilani zprav klientovi, tato trida je soucast GameControlleru a je funkcni pouze
 * s GameControllerem - instance se spolu s instanci GameControlleru
 */
class GameMessageHandler {

        /**
         * Reference na GameController
         */
        GameController& gameController;

    public:
        /**
         * Konstruktor pro vytvoreni - je potreba existujici vytvoreny GameController
         * @param gameController reference na GameController
         */
        explicit GameMessageHandler(GameController& gameController);

        /**
         * Posle klientovi pozadavek pro potvrzeni ucasti - tzn. spusteni dialogu se sazkou u klienta
         * @param client klient, kteremu zpravu zasilame
         */
        static void sendConfirmParticipationRequest(const shared_ptr<Client>& client);

        /**
         * Posle oznameni, ze hra nemohla odstartoval
         * @param client klient, kteremu zpravu zasilame
         */
        static void sendGameCouldNotStart(const shared_ptr<Client>& client);

        /**
         * Posle pozadavek o pripraveni hry
         * @param client klient, kteremu zpravu zasilame
         */
        static void sendPrepareGameRequest(const shared_ptr<Client>& client);

        /**
         * Posle vsechny informace o hre
         * @param player klient, kteremu zpravu zasilame
         * @param players vsichni hraci
         * @param dealer dealer
         */
        static void sendBoard(const shared_ptr<Client>& player, const vector<shared_ptr<Client>>& players,
                              const shared_ptr<Dealer>& dealer);

        /**
         * Posle hraci pozadavek pro zahrani
         * @param client klient, kteremu zpravu zasilame
         */
        static void sendPlayerTurnRequest(const shared_ptr<Client>& client);

        /**
         * Posle pozadavek pro zobrazeni hrace, ktery je na rade
         * @param client klient, kteremu zpravu zasilame
         * @param username jmeno hrace
         */
        static void sendCurrentPlayer(const shared_ptr<Client>& client, const string& username);

        /**
         * Posle klientovi, ze hrac byl preskocen kvuli neaktivite
         * @param client klient, kteremu zpravu zasilame
         * @param username uzivatel, ktery byl preskocen
         */
        static void sendPlayerSkipped(const shared_ptr<Client>& client, const string& username);

        /**
         * Zasle hraci zpravu ze neni na rade
         * @param client klient, kteremu zpravu zasilame
         */
        static void sendNotYourTurn(const shared_ptr<Client>& client);

        /**
         * Zasle hraci pozadavek pro zobrazeni vysledku tahu
         * @param turnResult vysledek tahu
         * @param client klient, kteremu zpravu zasilame
         */
        static void sendPlayerTurnResult(const shared_ptr<TurnResult>& turnResult, const shared_ptr<Client>& client);

        /**
         * Posle hraci zpravu ze zahral double down po zahrani hitu
         * @param client klient, kteremu zpravu zasilame
         */
        static void sendDoubleDownAfterHit(const shared_ptr<Client>& client);

        /**
         * Posle vsem hracum vysledky hry
         * @param clients vsichni hraci ve hre
         * @param dealer dealer
         */
        static void sendResults(const vector<shared_ptr<Client>>& clients, const shared_ptr<Dealer>& dealer);

        /**
         * Zasle zpravu o tom ze hra skoncila
         * @param client klient, kteremu zpravu zasilame
         */
        static void sendGameFinished(const shared_ptr<Client>& client);

        /**
         * Zasle klientovi pozadavek o zobrazeni reconnectu hrace
         * @param client klient, kteremu zpravu zasilame
         * @param username uzivatel, ktery se znovu pripojil
         */
        static void sendShowPlayerReconnected(const shared_ptr<Client>& client, const string& username);

        /**
         * Serializuje vysledek pro poslani po siti
         * @param clients vsichni hraci ve hre
         * @param dealer dealer
         * @return vrati string, ktery je serializovany a pripraveny pro odeslani pres tcp
         */
        static string
        getSerializedGameResult(const vector<shared_ptr<Client>>& clients, const shared_ptr<Dealer>& dealer);

        /**
         * Posle vysledky hry hraci
         * @param client klient, kteremu zpravu zasilame
         * @param clients vsichni hraci ve hre
         * @param dealer dealer
         */
        static void sendResults(const shared_ptr<Client>& client, const vector<shared_ptr<Client>>& clients,
                                const shared_ptr<Dealer>& dealer);

        /**
         * Odesle jakykoliv string na dany socket
         * @param clientSocket cislo socketu prijemce
         * @param message zprava, kterou posilame
         */
        static void sendMessage(int clientSocket, const string& message);
};


#endif //UPSEM_GAMEMESSAGEHANDLER_H
