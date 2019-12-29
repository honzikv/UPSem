//
// Created by nvk on 12/29/2019.
//

#ifndef UPSEM_DESERIALIZATIONEXCEPTION_H
#define UPSEM_DESERIALIZATIONEXCEPTION_H

#include <bits/exception.h>
#include <string>

using namespace std;

class DeserializationException : public exception {

        string message;

    public:
        explicit DeserializationException(string message = "Error while deserializing");

        const char* what();

};


#endif //UPSEM_DESERIALIZATIONEXCEPTION_H
