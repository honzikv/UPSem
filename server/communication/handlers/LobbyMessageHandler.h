//
// Created by itznu on 12/25/2019.
//

#ifndef UPSEM_LOBBYMESSAGEHANDLER_H
#define UPSEM_LOBBYMESSAGEHANDLER_H


#include "../../client/Client.h"
#include "../../Server.h"
#include "../serialization/TCPData.h"

class Lobby;

/**
 * Trida, ktera odesila zpravy klientum v lobby. Vyzaduje existujici lobby, MessageHandler zije stejne dlouho jako zije
 * instance Lobby
 */
class LobbyMessageHandler {

        /**
         * Reference na lobby, pro kterou instance zpracovava zpravy
         */
        Lobby& lobby;

    public:
        /**
         * Konstruktor pro vytvoreni instance
         * @param lobby reference na lobby
         */
        explicit LobbyMessageHandler(Lobby& lobby);

        /**
         * Zpracuje zpravu od klienta
         * @param client klient, ktery zpravu poslal
         * @param message zprava
         */
        void handleMessage(const shared_ptr<Client>& client, const shared_ptr<TCPData>& message);

        /**
         * Metoda pro zpracovani odpovedi od klienta
         * @param client klient, od ktereho se zprava prijala
         * @param response prijata odpoved
         */
        void handleResponse(const shared_ptr<Client>& client, const shared_ptr<TCPData>& response);

        /**
         * Metoda pro zpracovani pozadavku od klienta
         * @param client klient, od ktereho se zprava prijala
         * @param request prijaty pozadavek
         */
        void handleRequest(const shared_ptr<Client>& client, const shared_ptr<TCPData>& request);

        /**
         * Posle pozadavek pro zobrazeni, ze se pripojil novy hrac
         * @param client klient, kteremu se zprava posle
         * @param connectedPlayer pripojeny hrac
         */
        static void sendShowPlayerConnectedRequest(const shared_ptr<Client>& client, const string& connectedPlayer);

        /**
         * Posle pozadavek pro zobrazeni, ze se hrac odpojil
         * @param client klient, kteremu se zprava posle
         * @param disconnectedPlayer jmeno odpojeneho hrace
         */
        static void sendShowPlayerDisconnectedRequest(const shared_ptr<Client>& client, const string& disconnectedPlayer);

        /**
         * Posle jakykoliv string do daneho socketu
         * @param socket socket, do ktereho se zprava posle
         * @param message reference na zpravu
         */
        static void sendMessage(int socket, const string& message);

        /**
         * Posle pozadavek pro aktualizaci seznamu hracu vsem aktualne pripojenym klientum
         */
        void sendUpdatePlayerListRequest();

        /**
         * Odesle klientovi zpravu o tom ze nepotvrdil ucast
         * @param client klient, ktery nepotvrdil ucast
         */
        static void sendClientDidntConfirm(const shared_ptr<Client>& client);

        /**
         * Posle klientovi pozadavek o zobrazeni Lobby
         * @param client klient, kteremu zpravu zasilame
         */
        static void sendShowLobbyRequest(const shared_ptr<Client>& client);

        /**
         * Posle vsem klientum krome {@code exclude} aby si aktualizovali seznam hracu
         * @param exclude klient, kteremu zpravu neposilame
         */
        void sendUpdatePlayerListRequest(const shared_ptr<Client>& exclude);

        /**
         * Vytvori seznam hracu jako serializovany string, ktery lze poslat pres tcp
         * @return serializovany string seznamu vsech hracu pro zaslani pres tcp
         */
        string playerListSerializedString();

        /**
         * Zasle klientovi seznam vsech hracu
         * @param client klient, kteremu zpravu zasilame
         */
        void sendClientPlayerList(const shared_ptr<Client>& client);

        static void sendDeclineAcknowledged(const shared_ptr<Client>& client);
};


#endif //UPSEM_LOBBYMESSAGEHANDLER_H
