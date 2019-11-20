//
// Created by itznukeey on 20.11.19.
//

#ifndef UPSEM_JSONPARSER_H
#define UPSEM_JSONPARSER_H
#include "../libs/nlohmann/json.hpp"

using json = nlohmann::json;
using namespace std;

/**
 * Jednoducha trida, vyuzivajici knihovnu nlohmann json pro snadne parsovani jsonu
 */
class JSONParser {

public:
    string parseValue(string field, string message);

};


#endif //UPSEM_JSONPARSER_H
