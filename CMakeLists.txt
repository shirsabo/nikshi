cmake_minimum_required(VERSION 3.15)
project(ex3)

set(CMAKE_CXX_STANDARD 14)

add_executable(ex3 main.cpp Command.h OpenServerCommand.cpp OpenServerCommand.h ConnectCommand.h DefineVarCommand.h DefineVarCommand.cpp ConnectCommand.cpp)