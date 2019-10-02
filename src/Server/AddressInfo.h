//
// Created by itznu on 10/02/2019.
//

#ifndef UPSEM_ADDRESSINFO_H
#define UPSEM_ADDRESSINFO_H

#include <cstdio>
#include <string>

using namespace std;

class AddressInfo {
    int flags;

    int family;

    int socketType;

    int protocol;

    size_t addressLength;

    socketAddress& socketAddress;

    string canonName;
};


#endif //UPSEM_ADDRESSINFO_H
