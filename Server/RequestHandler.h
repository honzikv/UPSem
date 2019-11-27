//
// Created by itznukeey on 27.11.19.
//

#ifndef UPSEM_REQUESTHANDLER_H
#define UPSEM_REQUESTHANDLER_H

#include "Server.h"

#define MESSAGE_BUFFER_BYTES 1024
#define REQUEST "request"
#define LOGIN_REQUIRED "loginRequired"
#define LOGIN_FIELD "login"
#define PLAYER_COUNT "playerCount"

/**
 * Obsahuje metody pro komunikaci mezi klientem a serverem
 */
class RequestHandler {

    unique_ptr<JSONParser> jsonParser;

    shared_ptr<char[]> buffer;


private:
    void fillBuffer(string message) {
        strcpy(buffer.get(), message.c_str());
    }

    void clearBuffer() {
        memset(buffer.get(), 0, MESSAGE_BUFFER_BYTES);
    }

public:
    RequestHandler();

    /**
     * @param fileDescriptor filedescriptor klienta
     * @param field pole v jsonu
     * @param value hodnota pole v jsonu
     * @return boolean - pokud se vse poslalo v pohode vraci true, pokud ne vraci false
     */
    bool sendSingleParameterJSON(int fileDescriptor, const string& field, const string& value) {
        auto jsonString = jsonParser->serializeToJSON(field, value);
        this->fillBuffer(jsonString);
        return send(fileDescriptor, buffer.get(), jsonString.size() + 1, 0) >= 0;
    }

    bool sendMultiParameterJSON(int fileDescriptor, const vector<string>& fields, const vector<string>& values) {
        if (fields.size() != values.size()) {
            return false;
        }

        auto jsonString = jsonParser->serializeToJSON()
    }

    string parseClientRequest(int fileDescriptor) {
        return read(fileDescriptor, buffer.get(), MESSAGE_BUFFER_BYTES) <= 0
               ? "" : jsonParser->parseStringValue(REQUEST, buffer.get());
    }

};


#endif //UPSEM_REQUESTHANDLER_H
