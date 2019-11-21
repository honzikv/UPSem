//
// Created by itznukee on 11/20/19.
//

#ifndef UPSEM_CLIENT_H
#define UPSEM_CLIENT_H

#include <stdlib.h>
#include <string>
#include <netinet/in.h>

using namespace std;

class Client {

    string id;

    int fileDescriptor;

public:
    const string &getId() const;

public:
    Client(string id, int fileDescriptor);

};



#endif //UPSEM_CLIENT_H
