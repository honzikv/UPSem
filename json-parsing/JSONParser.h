//
// Created by itznukeey on 20.11.19.
//

#ifndef UPSEM_JSONPARSER_H
#define UPSEM_JSONPARSER_H
#include "../libs/nlohmann/json.hpp"



using JSON = nlohmann::json;
using namespace std;

/**
 * Jednoducha trida, vyuzivajici knihovnu nlohmann json pro snadne parsovani jsonu
 */
class JSONParser {

public:
    string parseStringValue(const string& field, const string& message);

    //TODO test
    string serializeToJSON(const string& field, const string& value) {
        JSON json;
        json[field] = value;

        return json.get<string>();
    }

    string serializeToJSON(const vector<string> fields, const vector<string> values) {
        JSON json;

        for (auto i = 0; i < fields.size(); i++) {
            json[fields[i]] = values[i];
        }

        return json.get<string>();
    }

    JSON createJSON()

};


#endif //UPSEM_JSONPARSER_H
