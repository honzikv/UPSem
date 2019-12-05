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
    void append(const string &field, const string &value);

    void clear();

    string serialize();
};


#endif //UPSEM_SERIALIZER_H
