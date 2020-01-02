//
// Created by nvk on 12/29/2019.
//

#include <algorithm>
#include "GamePreparation.h"

void GamePreparation::addConfirmedClient(shared_ptr<Client> client) {
    //Pokud vektor uz klienta neobsahuje prida ho (mel by se pridat vzdy, pouze pro zaruceni unikatnosti ve vektoru
    if (find(confirmedClients.begin(),confirmedClients.end(),client) == confirmedClients.end()) {
        confirmedClients.push_back(client);
    }
}

bool GamePreparation::isPlayable() {
    return confirmedClients.size() >= 2;
}

GamePreparation::GamePreparation() {
    initializationTime = chrono::system_clock::now();
}

const chrono::time_point<chrono::system_clock>& GamePreparation::getInitializationTime() const {
    return initializationTime;
}

const vector<shared_ptr<Client>>& GamePreparation::getConfirmedClients() const {
    return confirmedClients;
}
