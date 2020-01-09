//
// Created by itznukeey on 20.11.19.
//

#ifndef UPSEM_LOBBY_H
#define UPSEM_LOBBY_H
#define MAX_CLIENTS_PER_LOBBY 6

#include <cstdlib>
#include <vector>
#include <memory>
#include <thread>
#include <algorithm>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <queue>
#include "../client/Client.h"
#include "LobbyState.h"
#include "../communication/serialization/TCPData.h"
#include "../client/ClientData.h"
#include "../../Game/Blackjack.h"

using namespace std;
using namespace std::chrono;

class Server;

class LobbyMessageHandler;

class GameController;

class Lobby {

        /**
         * Nezpracovane zpravy
         */
        queue<shared_ptr<ClientData>> unprocessedMessages;

        /**
         * Seznam vsech klientu
         */
        vector<shared_ptr<Client>> clients;

        /**
         * Seznam klientu, kteri se behem hry odpojili
         */
        vector<shared_ptr<Client>> disconnectedClients;

        /**
         * Limit hracu
         */
        const int limit;

        /**
         * Id lobby, integer >= 0
         */
        const int id;

        /**
         * Pocet uzivatelu, kteri volili pro brzky start hry
         */
        int clientsReady = 0;

        /**
         * Aktualni stav lobby
         */
        LobbyState lobbyState = LOBBY_STATE_WAITING;

        /**
         * Zpracovavani zprav pro instanci lobby
         */
        shared_ptr<LobbyMessageHandler> lobbyMessageHandler;

        /**
         * Game controller
         */
        shared_ptr<GameController> gameController = nullptr;

        /**
         * Time point kdy se ukoncila hra - pro zobrazeni vysledku po urcitou dobu a pote odeslani pozdavku na navrat
         * do lobby
         */
        time_point<system_clock> returnToLobbyStart;

    public:
        /**
         * Vychozi konstruktor, vytvori lobby s limitem hracu a id
         * @param limit limit hracu v lobby
         * @param id unikatni identifikator lobby, cele cislo >= 0
         */
        Lobby(int limit, int id);

        /**
         * Getter pro id lobby
         * @return id lobby
         */
        int getId() const;

        /**
         * Getter pro stav lobby
         * @return stav lobby
         */
        LobbyState getLobbyState() const;

        /**
         * Setter pro stav lobby
         * @param lobbyState stav lobby, ktery chceme nastavit
         */
        void setLobbyState(LobbyState lobbyState);

        /**
         * Obnovi stav klienta
         * @param client klient, kteremu obnovujeme stav
         */
        void restoreState(const shared_ptr<Client>& client);

        /**
         * Vrati, zda-li je lobby dostupna pro pripjeni
         * @return
         */
        bool isJoinable() const;

        /**
         * Prida shared pointer na instanci klienta do lobby
         * @param client shared pointer klienta
         * @return zda-li se klient pridal
         */
        void addClient(shared_ptr<Client> client);

        /**
         * Odstrani shared pointer na instanci klienta z lobby
         * @param client reference na shared pointer instanci klienta
         */
        void removeClient(const shared_ptr<Client>& client);

        /**
         * Vrati pocet pripojenych klientu
         * @return  vrati pocet pripojenych klientu
         */
        int getClientCount();

        /**
         * Vytvori string s popisem lobby pro klienta
         * @return string s popisem lobby pro poslani zpravy klientovi
         */
        string toString();

        /**
         * Zvysi pocet hracu, kteri jsou pripraveni na hru
         */
        void incrementPlayersReady();

        /**
         * Resetuje vsem klientum ucast ve hre
         */
        void resetClientParticipation();

        /**
         * Vrati, zda-li lobby obsahuje klienta
         * @param client klient, ktereho hledame
         * @return true - obsahuje, false - neobsahuje
         */
        bool contains(const shared_ptr<Client>& client);

        /**
         * Prida na frontu novou zpravu ke zpracovani
         * @param message zprava, kterou ma lobby zpracovat
         * @param client klient, ktery zpravu odeslal
         */
        void addNewMessage(const shared_ptr<TCPData>& message, const shared_ptr<Client>& client);

        /**
         * Zkontroluje stav mistnosti, pokud se hraje hra, zkontroluje zda hra stale bezi a adekvatne upravi stav
         */
        void handleLobby();

        /**
         * Vrati seznam klientu v lobby
         * @return vrati seznam vsech klientu v lobby
         */
        const vector<shared_ptr<Client>>& getClients() const;

        /**
         * Posle klientovi zpravu ze nepotvrdil ucast
         * @param client klient, ktery nepotvrdil ucast
         */
        void sendClientDidntConfirm(const shared_ptr<Client>& client);

        /**
         * Posle vsem klientum zpravu o odpojeni klienta
         * @param client klient, ktery se odpojil
         */
        void sendClientDisconnected(const shared_ptr<Client>& client);

        /**
         * Zkontroluje jestli je cas se vratit do lobby po zobrazeni vysledku hry
         */
        void checkIfReturnToLobby();

        /**
         * Vrati referenci na shared pointer game controlleru
         * @return Vrati referenci na shared pointer game controlleru
         */
        const shared_ptr<GameController>& getGameController() const;

        /**
         * Nastavi cas pro navrat zpet do lobby
         */
        void prepareToReturn();

        /**
         * Zjisti, zda-li se muze vsem klientum poslat pozadavek na sazku
         * @return true pokud muze priprava zacit
         */
        bool canPrepareGameStart();

        /**
         * Prida odpojeneho klienta pro pozdejsi odstraneni z lobby pokud se nepripoji zpet
         * @param client klient, ktery se odpojil behem hry
         */
        void addDisconnectedClient(const shared_ptr<Client>& client);
};


#endif //UPSEM_LOBBY_H
