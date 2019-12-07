//
// Created by nvk on 12/05/2019.
//

#include "Serializer.h"

void Serializer::append(const string& field, const string& value) {
    fields.emplace(field, value);
}

void Serializer::clear() {
    fields.clear();
}

string Serializer::serialize() {
    auto data = string("{");

    for (auto const &[field, value] : fields) {
        data.append(field).append(":").append(value).append(",");
    }
    //smazani posledni carky
    data.erase(data.length() - 1, data.length() - 1);
    data.append("}\n");

    clear(); //Smaze vsechny data
    return data;
}
