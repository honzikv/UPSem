cmake_minimum_required(VERSION 3.14)
project(UPSem)

set(CMAKE_CXX_STANDARD 17)

add_executable(UPSem main.cpp Server/Server.cpp Server/Server.h Server/Client.cpp Server/Client.h Server/Lobby.cpp Server/Lobby.h Game/Player.cpp Game/Player.h Game/Card.cpp Game/Card.h Game/Blackjack.cpp Game/Blackjack.h Game/Deck.cpp Game/Deck.h Server/serialization/Deserializer.cpp Server/serialization/Deserializer.h Server/serialization/Serializer.cpp Server/serialization/Serializer.h Server/communication/MessageSender.cpp Server/communication/MessageSender.h Server/communication/MessageReceiver.cpp Server/communication/MessageReceiver.h Server/communication/Constants.h)