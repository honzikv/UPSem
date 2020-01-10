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
        /**
         * Konstruktor pro vytvoreni GameControlleru. Instance zanika spolecne s lobby
         * @param lobby lobby, ktere GameController patri
         */
        explicit GameController(Lobby& lobby);

        /**
         * Zacne s pripravou hry
         */
        void prepareGame();

        /**
         * Prida noveho klienta, ktery potvrdil ze bude hrat hru
         * @param client shared pointer klienta
         * @param bet sazka
         */
        void addConfirmedClient(shared_ptr<Client> client, int bet);

        /**
         * Prepoji klienta
         * @param client shared pointer klienta
         */
        void reconnectClient(const shared_ptr<Client>& client);

        /**
         * Vrati, zda-li je hra hratelna - tzn. pocet klientu, kteri potvrdili sazku je alespon 2
         * @return true pokud ano, jinak ne
         */
        bool isPlayable();

        /**
         * Zrusi start hry
         */
        void cancelGame();

        /**
         * Odstartuje hru
         */
        void startGame();

        /**
         * Posle vsem hracum board update
         */
        void sendBoardUpdate();

        /**
         * Prida odpojeneho klienta jako autoskip - klient se umyslne odpojil a nechce hrat
         * @param client shared pointer na klienta
         */
        void addAutoSkip(shared_ptr<Client> client);

        /**
         * Zkontroluje jestli je cas na dalsiho hrace a pokud ano provede adekvatni operaci
         */
        void checkIfTimeForNextPlayer();

        /**
         * Posle vsem aktualne hrajiciho hrace
         */
        void sendCurrentPlayer();

        /**
         * Zpracuje hracuv tah
         * @param player reference na shared pointer hrace
         * @param message zprava od hrace
         */
        void handlePlayerTurn(const shared_ptr<Client>& player, const shared_ptr<TCPData>& message);

        /**
         * Ukonci hru
         */
        void endGame();

        /**
         * Vrati, zda-li je hra dokoncena
         * @return true pokud hra skoncila, jinak false
         */
        bool isGameFinished();

        /**
         * Vrati, zda-li priprava hry - tzn. sazky skoncila
         * @return true pokud je uzavirani sazek u konce
         */
        bool hasPreparationTimeExpired();

        /**
         * Zkontroluje, jestli hra obsahuje klienta
         * @param client reference na klienta
         * @return vrati true, pokud klienta obsahuje
         */
        bool containsConfirmedClient(const shared_ptr<Client>& client);

        /**
         * Odstrani vsechny herni data
         */
        void removeData();

        /**
         * Zkontroluje, jestli ma hrace automaticky preskocit
         * @param client hrac, ktereho ma automaticky preskocit
         * @return true pokud ano
         */
        bool autoSkipPlayer(const shared_ptr<Client>& client);
};


#endif //UPSEM_GAMECONTROLLER_H
