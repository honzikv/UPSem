//
// Created by itznukeey on 27.11.19.
//

#include "RequestHandler.h"

RequestHandler::RequestHandler() {
    this->jsonParser = make_unique<JSONParser>();
    this->buffer = make_shared<char[]>(new char[MESSAGE_BUFFER_BYTES]);
}
