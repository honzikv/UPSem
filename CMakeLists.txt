cmake_minimum_required(VERSION 3.14)
project(UPSem)

set(CMAKE_CXX_STANDARD 17)

add_executable(UPSem main.cpp Server/Server.cpp Server/Server.h Server/Client.cpp Server/Client.h Server/Lobby.cpp Server/Lobby.h json-parsing/JSONParser.cpp json-parsing/JSONParser.h Server/ThreadFunctions.cpp Server/ClientThread.cpp Server/ClientThread.h Game/Player.cpp Game/Player.h Game/Card.cpp Game/Card.h Game/Blackjack.cpp Game/Blackjack.h Game/Deck.cpp Game/Deck.h)