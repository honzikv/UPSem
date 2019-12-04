//
// Created by itznukee on 11/20/19.
//

#ifndef UPSEM_CLIENT_H
#define UPSEM_CLIENT_H

#include <cstdlib>
#include <string>
#include <netinet/in.h>

using namespace std;

class Client {

    string id;

    int fileDescriptor;

public:
    Client(string id, int fileDescriptor);
    const string &getId() const;

};



#endif //UPSEM_CLIENT_H
