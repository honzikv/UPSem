//
// Created by nvk on 12/04/2019.
//

#ifndef UPSEM_DESERIALIZER_H
#define UPSEM_DESERIALIZER_H

#include <cstdlib>
#include <cstring>
#include <string>
#include <memory>
#include <unordered_map>
#include <regex>
#include <iostream>

using namespace std;

/**
 * Format dat - jedna se de facto o json, pouze pole a hodnoty neobsahuji uvozovky
 * Program nepouziva nested objekty - tzn hodnota nemuze byt dalsi objekt
 * objekt -> {field1:value1,field2:value2,field3:value3}
 */
class Deserializer {

private:
    /**
     * Data, ktera se budou parsovat
     */
    unique_ptr<string> data;

    /**
     * Hashmapa, ktera uklada vsechna pole
     */
    unordered_map<string, string> fields;

public:
    void deserialize(const char *charArray);

    string valueOf(const string &field);
};


#endif //UPSEM_DESERIALIZER_H
