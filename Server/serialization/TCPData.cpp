//
// Created by nvk on 12/08/2019.
//

#include "TCPData.h"

void TCPData::deserialize(string message) {
    auto data = make_unique<string>(message);
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
        if ((doubleColonPosition = field.find(':')) == string::npos || doubleColonPosition == field.length() - 1) {
            cout << "incorrect input" << endl;
            throw DeserializationException();
        }

        this->fields.emplace(field.substr(0, doubleColonPosition),
                             field.substr(doubleColonPosition + 1, field.size() - 1));
    }
}

string TCPData::serialize() {
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
    return data;
}

void TCPData::add(const string& field, const string& value) {
    if (!isEditable) {
        throw exception();
    }

    fields.emplace(field, value);
}

string TCPData::valueOf(const string& field) {
    return fields.at(field);
}

TCPData::TCPData(const string& message) {
    deserialize(message);
}

TCPData::TCPData(DataType dataType) {
    this->dataType = dataType;
}
