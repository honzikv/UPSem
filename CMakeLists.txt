cmake_minimum_required(VERSION 3.14)
project(UPSem)

set(CMAKE_CXX_STANDARD 17)
find_package( Threads REQUIRED)
set(CMAKE_CXX_FLAGS -pthread)

add_executable(UPSem main.cpp Server/Server.cpp Server/Server.h Server/Client.cpp Server/Client.h Server/lobby/Lobby.cpp Server/lobby/Lobby.h Game/Card.cpp Game/Card.h Game/Blackjack.cpp Game/Blackjack.h Game/Deck.cpp Game/Deck.h Server/communication/Constants.h Server/serialization/TCPData.cpp Server/serialization/TCPData.h Server/serialization/DataType.h Server/communication/Fields.h Server/communication/Values.h Server/lobby/LobbyState.h Server/lobby/LobbyMessageHandler.cpp Server/lobby/LobbyMessageHandler.h Server/communication/ClientData.h Server/communication/ClientData.cpp Server/communication/MessageHandler.h Server/communication/MessageHandler.cpp Server/serialization/exceptions/DeserializationException.cpp Server/serialization/exceptions/DeserializationException.h Server/lobby/GamePreparation.cpp Server/lobby/GamePreparation.h Game/PlayerInfo.cpp Game/PlayerInfo.h Game/Result.h Game/Dealer.cpp Game/Dealer.h Game/TurnResult.cpp Game/TurnResult.h)
target_link_libraries( UPSem ${CMAKE_THREAD_LIBS_INIT} )