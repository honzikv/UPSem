//
// Created by nvk on 12/04/2019.
//

#include "Deserializer.h"

void Deserializer::deserialize(string &data) {
    data.erase(remove(data.begin(), data.end(), '{'), data.end());
    data.erase(remove(data.begin(), data.end(), '}'), data.end());
    cout << data.c_str() << endl;

    auto fieldList = vector<string>();
    auto currentPosition = 0;
    auto lastPosition = 0;

    auto stringStream = stringstream(data);

    string temp;
    while (getline(stringStream, temp, ',')) {
        fieldList.push_back(temp);
    }

    for (const auto &field : fieldList) {
        int doubleColonPosition;
        if ((doubleColonPosition = field.find(':')) == string::npos ||
            doubleColonPosition == field.length() - 1) {
            throw exception();
        }

        this->fields.emplace(field.substr(0, doubleColonPosition),
                             field.substr(doubleColonPosition + 1, field.size() - 1));
    }
}

void Deserializer::parse(const char *charArray) {
    this->data = make_unique<string>(string(charArray));
    fields.clear();

    //Parsing objektu
    if (data->find_first_of("{") == 0 && data->find_last_of("}") == data->length() - 1) {
        deserialize(this->data.operator*());
    }
}

string Deserializer::getValue(const string &field) {
    return fields.at(field);
}
