cmake_minimum_required(VERSION 3.14)
project(UPSem)

set(CMAKE_CXX_STANDARD 17)

add_executable(UPSem main.cpp Server/Server.cpp Server/Server.h Server/Client.cpp Server/Client.h)