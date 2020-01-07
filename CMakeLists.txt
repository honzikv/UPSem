cmake_minimum_required(VERSION 3.0)
project(UPSServer)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS)

add_executable(UPSServer
        main.cpp
        server/Server.cpp
        server/Server.h
        server/client/Client.cpp
        server/client/Client.h
        server/lobby/Lobby.cpp
        server/lobby/Lobby.h
        game/Card.cpp
        game/Card.h
        game/Blackjack.cpp
        game/Blackjack.h
        game/Deck.cpp
        game/Deck.h
        server/communication/Constants.h
        server/communication/serialization/TCPData.cpp
        server/communication/serialization/TCPData.h
        server/communication/serialization/DataType.h
        server/communication/Fields.h
        server/communication/Values.h
        server/lobby/LobbyState.h
        server/communication/handlers/LobbyMessageHandler.cpp
        server/communication/handlers/LobbyMessageHandler.h
        server/client/ClientData.h
        server/client/ClientData.cpp
        server/communication/handlers/MessageHandler.h
        server/communication/handlers/MessageHandler.cpp
        server/communication/serialization/exceptions/DeserializationException.cpp
        server/communication/serialization/exceptions/DeserializationException.h
        server/lobby/GamePreparation.cpp
        server/lobby/GamePreparation.h
        server/client/player/PlayerInfo.cpp
        server/client/player/PlayerInfo.h
        game/Result.h
        game/Dealer.cpp
        game/Dealer.h
        game/TurnResult.cpp
        game/TurnResult.h
        server/communication/handlers/GameMessageHandler.cpp
        server/communication/handlers/GameMessageHandler.h
        server/lobby/controller/GameController.cpp
        server/lobby/controller/GameController.h)
target_link_libraries(UPSServer ${CMAKE_THREAD_LIBS_INIT})