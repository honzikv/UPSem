//
// Created by nvk on 12/07/2019.
//

#include "MessageReceiver.h"

MessageReceiver::MessageReceiver(int fileDescriptor, Server& server) : fileDescriptor(fileDescriptor), server(server) {
    this->deserializer = make_unique<Deserializer>();
}
