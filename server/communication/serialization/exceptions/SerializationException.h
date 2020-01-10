//
// Created by nvk on 12/29/2019.
//

#ifndef UPSEM_DESERIALIZATIONEXCEPTION_H
#define UPSEM_DESERIALIZATIONEXCEPTION_H

#include <bits/exception.h>
#include <string>

using namespace std;

/**
 * Chyba pri (de)serializaci
 */
class SerializationException : public exception {

        string message;

    public:
        explicit SerializationException(string message = "Error while deserializing");

};


#endif //UPSEM_DESERIALIZATIONEXCEPTION_H
