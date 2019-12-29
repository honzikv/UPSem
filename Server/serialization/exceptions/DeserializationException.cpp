//
// Created by nvk on 12/29/2019.
//

#include "DeserializationException.h"

DeserializationException::DeserializationException(string message) : message(message){
}

const char* DeserializationException::what() {
    return message.c_str();
}
