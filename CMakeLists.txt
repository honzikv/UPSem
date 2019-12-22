cmake_minimum_required(VERSION 3.14)
project(UPSem)

set(CMAKE_CXX_STANDARD 17)
find_package( Threads )

add_executable(UPSem main.cpp Server/Server.cpp Server/Server.h Server/Client.cpp Server/Client.h Server/Lobby.cpp Server/Lobby.h Game/Player.cpp Game/Player.h Game/Card.cpp Game/Card.h Game/Blackjack.cpp Game/Blackjack.h Game/Deck.cpp Game/Deck.h Server/communication/Constants.h Server/serialization/TCPData.cpp Server/serialization/TCPData.h Server/serialization/DataType.h Server/communication/FieldConstants.h Server/communication/ValueConstants.h)
target_link_libraries( UPSem ${CMAKE_THREAD_LIBS_INIT} )