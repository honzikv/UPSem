cmake_minimum_required(VERSION 3.14)
project(UPSem)

set(CMAKE_CXX_STANDARD 17)

add_executable(UPSem main.cpp src/Server/Main.cpp src/Server/AddressInfo.cpp src/Server/AddressInfo.h src/Server/socketAddress.cpp src/Server/socketAddress.h src/InternetAddress.cpp src/InternetAddress.h)