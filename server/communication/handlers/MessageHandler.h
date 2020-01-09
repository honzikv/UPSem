//
// Created by nvk on 12/26/2019.
//

#ifndef UPSEM_MESSAGEHANDLER_H
#define UPSEM_MESSAGEHANDLER_H

#include "../../client/Client.h"
#include "../../lobby/Lobby.h"
#include "../serialization/TCPData.h"

class Server;

class MessageHandler {

        /**
         * Reference na server
         *
         */
        Server& server;


    public:
        explicit MessageHandler(Server& server);

        /**
         * Metoda pro prihlaseni klienta - reconnect a login.
         * @param clientSocket socket klienta
         * @param message zprava
         * @return vraci true, pokud byl klient uspesne pripojen, jinak se spojeni zavre
         */
        void handleMessage(int clientSocket, const shared_ptr<TCPData>& message);

    private:

        /**
         * Pingne klientovi zpet
         * @param clientSocket socket, ze ktereho ping zprava prisla
         */
        void pingBack(int clientSocket);

        /**
         * Posle na dany clientSocket zpravu {@code message}
         * @param clientSocket cislo socketu klienta
         * @param message zprava
         */
        void sendMessage(int clientSocket, const string& message);

        /**
         * Posle danemu klientovi lobby list
         * @param client klient, kteremu zpravu zasilame
         */
        void sendLobbyList(const shared_ptr<Client>& client);

        /**
         * Pro vetveni pozadavku
         * @param clientSocket cislo socketu klienta
         * @param message
         */
        void handleRequest(int clientSocket, const shared_ptr<TCPData>& message);

        /**
         * Zasle klientovi ze je login novy - tzn. nebude obnovovat stav
         * @param clientSocket cislo socketu klienta
         */
        void sendLoginIsNew(int clientSocket);

        /**
         * Pro vetveni odpovedi
         * @param clientSocket cislo socketu klienta
         * @param message zprava
         */
        void handleResponse(int clientSocket, const shared_ptr<TCPData>& message);

        /**
         * Posle informace o lobby
         * @param client klient, kteremu zpravu zasilame
         * @param message prijata zprava od klienta
         */
        void sendLobby(shared_ptr<Client>& client, const shared_ptr<TCPData>& message);

        /**
         * Odstrani klienta z lobby
         * @param client klient, ktery se odstrani z lobby
         * @param message zprava klienta
         */
        void leaveLobby(shared_ptr<Client>& client, const shared_ptr<TCPData>& message);

        /**
         * Obnovi stav klienta
         * @param client klient, kteremu se obnovi stav
         */
        void restoreClientState(const shared_ptr<Client>& client);

        /**
         * Zasle na socket ze se klient prihlasil z jineho socketu
         * @param clientSocket cilovy socket zpravy
         */
        void sendClientReconnectedFromAnotherLocation(int clientSocket);

        /**
         * Validace loginu
         * @param clientSocket socket klienta
         * @param message prijata zprava
         */
        void handleLogin(int clientSocket, const shared_ptr<TCPData>& message);

};


#endif //UPSEM_MESSAGEHANDLER_H
