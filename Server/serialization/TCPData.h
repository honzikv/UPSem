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
    explicit TCPData(DataType dataType) {
        this->dataType = dataType;
    }

    explicit TCPData(const char* charArray) {
        deserialize(charArray);
    }

    string valueOf(string field) {
        return fields.at(field);
    }

    void add(string field, string value) {
        if (!isEditable) {
            throw exception();
        }

        fields.emplace(field, value);
    }

    string serialize() {
        auto data = string("{");

        for (auto const &[field, value] : fields) {
            data.append(field).append(":").append(value).append(",");
        }
        data.append("dataType").append(":");

        if (dataType == DATATYPE_REQUEST) {
            data.append("request");
        } else if (dataType == DATATYPE_RESPONSE) {
            data.append("response");
        } else {
            data.append("ping");
        }

        data.append("}\n");

        isEditable = false;
        cout << data;
        return data;
    }

private:
    void deserialize(const char* charArray) {
        auto data = make_unique<string>(string(charArray));
        fields.clear();

        //Parsing objektu
        if (data->find_first_of("{") != 0 || data->find_last_of("}") != data->length() - 1) {
            cerr << "error while parsing data" << endl;
            throw exception();
        }

        data->erase(remove(data->begin(), data->end(), '{'), data->end());
        data->erase(remove(data->begin(), data->end(), '}'), data->end());

        auto fieldList = vector<string>();
        auto stringStream = stringstream(data.operator*());

        string temp;
        while (getline(stringStream, temp, ',')) {
            fieldList.push_back(temp);
        }

        for (const auto& field : fieldList) {
            int doubleColonPosition;
            if ((doubleColonPosition = field.find(':')) == string::npos ||
                doubleColonPosition == field.length() - 1) {
                cout << "incorrect input" << endl;
                throw exception();
            }

            this->fields.emplace(field.substr(0, doubleColonPosition),
                                 field.substr(doubleColonPosition + 1, field.size() - 1));
        }
    }
};


#endif //UPSEM_TCPDATA_H
