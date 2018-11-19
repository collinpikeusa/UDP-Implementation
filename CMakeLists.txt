cmake_minimum_required(VERSION 3.12)
project(project)

set(CMAKE_CXX_STANDARD 11)

add_executable(Client client.cpp)
add_executable(Server server.cpp)