//
// Created by nvk on 12/05/2019.
//

#ifndef UPSEM_SERIALIZER_H
#define UPSEM_SERIALIZER_H

#include <cstring>
#include <unordered_map>

using namespace std;

class Serializer {

private:
    unordered_map<string, string> fields;

public:
    void append(const string &field, const string &value) {
        fields.emplace(field, value);
    }

    void clear() {
        fields.clear();
    }

    string serialize() {
        auto data = string("{");

        for (auto const &[field, value] : fields) {
            data.append(field + ":" + value + ",");
        }
        //smazani posledni carky
        data.erase(data.length() - 1, data.length() - 1);
        data.append("}");

        clear(); //Smaze vsechny data
        return data;
    }
};


#endif //UPSEM_SERIALIZER_H
