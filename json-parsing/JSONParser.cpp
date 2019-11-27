//
// Created by itznukeey on 20.11.19.
//

#include "JSONParser.h"

using namespace std;

string JSONParser::parseStringValue(const string& field, const string& message) {
    try {
        JSON jsonString = message;
        string value = jsonString.value(field, "");
        return value;
    }
    catch (nlohmann::detail::type_error &ex) {
        throw invalid_argument("Not a valid response");
    }
}
