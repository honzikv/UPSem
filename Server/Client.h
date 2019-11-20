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

    sockaddr_in address;

public:
    Client(string id, const sockaddr_in &address);


};



#endif //UPSEM_CLIENT_H
