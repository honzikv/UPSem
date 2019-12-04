//
// Created by nvk on 12/04/2019.
//

#ifndef UPSEM_SERIALIZER_H
#define UPSEM_SERIALIZER_H

#include <cstdlib>
#include <cstring>
#include <string>
#include <memory>
#include <unordered_map>
#include <regex>

using namespace std;

/**
 * Format dat - jedna se de facto o json, pouze pole a hodnoty neobsahuji uvozovky "
 * Program nepouziva nested objekty - tzn hodnota nemuze byt dalsi objekt
 * objekt -> {field1:value1,field2:value2,field3:value3}
 * pole objektu [{field1:value,field2:value},{field1:value}]
 */
class Serializer {

private:
    /**
     * Data, ktera se budou parsovat
     */
    unique_ptr<string> data;

    unordered_map<string, string> fields;

    void parseObject(const unique_ptr<string>& data) {
        data->erase(remove(data->begin(), data->end(), '{'), data->end());
        data->erase(remove(data->begin(), data->end(), '}'), data->end());

        auto fieldList = vector<string>();
        auto currentPosition = 0;
        auto lastPosition = 0;

        while ((currentPosition = data->find(",")) != string::npos) {
            fieldList.push_back(data->substr(lastPosition, currentPosition));
            lastPosition = currentPosition;
        }

        for (const auto &field : fieldList) {
            int doubleColonPosition;
            if ((doubleColonPosition = field.find(':')) == string::npos ||
                doubleColonPosition == field.length() - 1) {
                throw exception();
            }

            this->fields.emplace(field.substr(0, doubleColonPosition - 1),
                           field.substr(doubleColonPosition + 1, field.size() - 1));
        }
    }

public:
    void parse(const char *charArray, int size) {
        this->data = make_unique<string>(string(charArray));
        fields.clear();

        //Parsing objektu
        if (data->find_first_of("{") == 0 && data->find_last_of("}") == data->length() - 1) {
            parseObject(this->data);
        }

        else if (data->find_first_of("[") == 0 && data->find_last_of("]") == data->length() - 1) {

        }
    }
};


#endif //UPSEM_SERIALIZER_H
