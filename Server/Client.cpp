//
// Created by itznukee on 11/20/19.
//

#include "Client.h"

#include <utility>

Client::Client(string id, const sockaddr_in &address) : id(std::move(id)), address(address) {}
