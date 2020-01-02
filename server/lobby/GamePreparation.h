//
// Created by nvk on 12/29/2019.
//

#ifndef UPSEM_GAMEPREPARATION_H
#define UPSEM_GAMEPREPARATION_H

#include "../client/Client.h"

using namespace std;

/**
 * Data transfer object pro ulozeni hodnot pro lepsi prehlednost kodu
 */
class GamePreparation {

        /**
         * Klienti, kteri potvrdili ze budou hrat
         */
        vector<shared_ptr<Client>> confirmedClients;

        /**
         * Cas, kdy lobby poslala vsem klientum pozadavek, zda-li se pripoji
         */
        chrono::time_point<chrono::system_clock> initializationTime;

    public:
        GamePreparation();

        void addConfirmedClient(shared_ptr<Client> client);

        bool isPlayable();

        const chrono::time_point<chrono::system_clock>& getInitializationTime() const;

        const vector<shared_ptr<Client>>& getConfirmedClients() const;
};


#endif //UPSEM_GAMEPREPARATION_H
