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

        bool isAuthenticated = false;

        bool voted = false;
    public:
        bool hasVoted() const;

        bool operator==(const Client& anotherClient) const;

        bool operator!=(const Client& anotherClient) const;

    public:
        Client(string id, int fileDescriptor);

        const string& getId() const;

        int getFileDescriptor() const;

        bool isAuthenticated1() const;

        void setIsAuthenticated(bool isAuthenticated);

        void setHasVoted(bool hasVoted);

};


#endif //UPSEM_CLIENT_H
