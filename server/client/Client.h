//
// Created by itznukee on 11/20/19.
//

#ifndef UPSEM_CLIENT_H
#define UPSEM_CLIENT_H

#include <cstdlib>
#include <string>
#include <netinet/in.h>
#include <atomic>
#include <chrono>
#include <utility>
#include <vector>
#include <memory>
#include "player/PlayerInfo.h"

using namespace std;

/**
 * Trida s informacemi o klientovi, ktery se autentifikoval na serveru
 */
class Client {

        /**
         * Id klienta, tzn. jeho username
         */
        const string username;

        /**
         * Cislo socketu klienta
         */
        int clientSocket;

        /**
         * Flag, zda-li uzivatel zvolil ze je pripraven pro start hry
         */
        bool ready = false;

        /**
         * Identifikator lobby -> -1 znaci ze klient v zadne lobby neni
         */
        int lobbyId = -1;

        /**
         * Cas posledni zpravy - pro odpojeni mrtveho pripojeni
         */
        chrono::time_point<chrono::system_clock> lastMessageReceived;

        /**
         * Info ve hre - ruka, vsazena castka
         */
        shared_ptr<PlayerInfo> playerInfo;

    public:
        /**
         * Konstruktor pro vytvoreni nove instance klienta
         * @param username prezdivka klienta
         * @param clientSocket aktualni socket
         */
        Client(string username, int clientSocket);

        /**
         * Vrati string s informacemi o klientovi
         * @return c++ string s informacemi o klientovi pro frontend
         */
        string toString();

        /**
         * Getter pro id lobby
         * @return vrati id lobby, ve je aktualne klient
         */
        int getLobbyId() const;

        /**
         * Getter pro uzivatelske jmeno
         * @return vrati uzivatelske jmeno klienta
         */
        const string& getUsername() const;

        /**
         * Getter pro aktualni socket klienta
         * @return vrati cislo socketu klienta
         */
        int getClientSocket() const;

        /**
         * Setter pro lobby id
         * @param lobbyId id nove lobby
         */
        void setLobbyId(int lobbyId);

        /**
         * Setter pro socket klienta - napr. kdyz se klient prepoji je potreba zmenit cislo socketu, protoze
         * pravdepodobne nezustalo stejne
         * @param clientSocket
         */
        void setClientSocket(int clientSocket);

        /**
         * Vrati, zda-li klient potvrdil ze je pripraven na hru
         * @return boolean zda-li je klient pripraven na hru
         */
        bool isReady() const;

        /**
         * Setter pro flag ready - zda-li je klient pripraven hrat
         * @param isReady boolean zda-li je hrac pripraveny hrat
         */
        void setReady(bool isReady);

        /**
         * Override operatoru == pro zajisteni, ze kdyz budeme porovnavat dva shared pointery tak se porovnaji i
         * hodnoty klientu
         * @param anotherClient klient, jehoz rovnost porovnavame s touto instanci
         * @return true pokud jsou instance rovne
         */
        bool operator==(const Client& anotherClient) const;

        /**
         * Override operatoru nerovnosti, pouze zneguje operator pro rovnost
         * @param anotherClient klient, jehoz nerovnost chceme zjistit
         * @return true pokud se instance nerovnaji
         */
        bool operator!=(const Client& anotherClient) const;

        /**
         * Aktualizuje cas posledni zpravy na aktualni cas systemu
         */
        void updateLastMessageReceived();

        /**
         * Getter pro ziskani casu posledni zpravy
         * @return referenci na time point posledniho casu zpravy
         */
        const chrono::time_point<chrono::system_clock>& getLastMessageReceived() const;

        /**
         * Getter pro informace o hraci
         * @return vrati referenci na shared pointer s informacemi o hraci
         */
        const shared_ptr<PlayerInfo>& getPlayerInfo() const;

        /**
         * Vytvori nove info o hraci se sazkou {@code bet}
         * @param bet sazka hrace
         */
        void createPlayerInfo(int bet);
};


#endif //UPSEM_CLIENT_H
