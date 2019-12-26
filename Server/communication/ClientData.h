//
// Created by nvk on 12/25/2019.
//

#ifndef UPSEM_CLIENTDATA_H
#define UPSEM_CLIENTDATA_H


#include "../serialization/TCPData.h"
#include "../Client.h"

/**
 * Wrapper pro data, aby je lobby mohla zpracovat
 */
class ClientData {

        shared_ptr<TCPData> message;

        shared_ptr<Client> client;

    public:
        ClientData(shared_ptr<TCPData>  message, shared_ptr<Client>  client);

        const shared_ptr<TCPData>& getMessage() const;

        const shared_ptr<Client>& getClient() const;

};


#endif //UPSEM_CLIENTDATA_H
