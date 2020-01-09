//
// Created by nvk on 12/25/2019.
//

#ifndef UPSEM_CLIENTDATA_H
#define UPSEM_CLIENTDATA_H


#include "../communication/serialization/TCPData.h"
#include "Client.h"

/**
 * Wrapper pro data, aby je lobby mohla zpracovat
 */
class ClientData {

        /**
         * Shared pointer na TCPData s deserializovanou zpravou
         */
        const shared_ptr<TCPData> message;

        /**
         * Shared pointer klienta
         */
        const shared_ptr<Client> client;

    public:
        /**
         * Vychozi konstruktor
         * @param message shared pointer na deserializovanou zpravu
         * @param client shared pointer na klienta
         */
        ClientData(shared_ptr<TCPData> message, shared_ptr<Client> client);

        /**
         * Getter zpravy
         * @return referenci na shared pointer se zpravou
         */
        const shared_ptr<TCPData>& getMessage() const;

        /**
         * Getter klienta
         * @return referenci na shared pointer s klientem
         */
        const shared_ptr<Client>& getClient() const;

};


#endif //UPSEM_CLIENTDATA_H
