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
#include "exceptions/SerializationException.h"

using namespace std;

/**
 * Trida, ktera se stara o serializaci a deserializaci stringu z bufferu
 */
class TCPData {

        /**
         * Typ zpravy
         */
        DataType dataType;

        /**
         * Hashmapa pro ulozeni poli a jejich hodnot
         */
        unordered_map<string, string> fields;

        /**
         * Flag zda-li je zprava upravitelna
         */
        bool isEditable = true;

    public:
        /**
         * Konstruktor pro serializaci
         * @param dataType typ zpravy - request nebo response
         */
        explicit TCPData(DataType dataType);

        /**
         * Konstruktor pro deserializaci zpravy ze stringu {@code message}
         * @param message zprava, ktera se ma zpracovat
         */
        explicit TCPData(const string& message);

        /**
         * Vrati hodnotu daneho pole
         * @param field nazev pole
         * @return string s hodnotou
         */
        string valueOf(const string& field);

        /**
         * Prida par pole hodnota
         * @param field nazev pole
         * @param value hodnota
         */
        void add(const string& field, const string& value);

        /**
         * serializuje zpravu a vrati vysledek jako string
         * @return serializovana zprava
         */
        string serialize();

    private:
        /**
         * Deserializuje string, pokud nelze hodi SerializationException
         * @param message zprava, kterou deserializujeme
         */
        void deserialize(string message);
};


#endif //UPSEM_TCPDATA_H
