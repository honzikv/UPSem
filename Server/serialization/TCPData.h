//
// Created by nvk on 12/08/2019.
//

#ifndef UPSEM_TCPDATA_H
#define UPSEM_TCPDATA_H


#include <cstdlib>
#include <cstring>
#include <string>
#include <memory>
#include <unordered_map>
#include <regex>
#include <iostream>
#include "DataType.h"

using namespace std;

class TCPData {

        DataType dataType;

        unordered_map<string, string> fields;

        bool isEditable = true;

    public:
        explicit TCPData(DataType dataType);

        /**
         * Predpoklada prectenou zpravu z bufferu, zpravy pouzivaji oddeleni {}\n
         * @param message zprava, ktera se ma zpracovat
         */
        explicit TCPData(const string& message);

        string valueOf(const string& field);

        void add(const string& field, const string& value);

        serialize();

        bool isValid() const;

    private:
        void deserialize(string message);
};


#endif //UPSEM_TCPDATA_H
