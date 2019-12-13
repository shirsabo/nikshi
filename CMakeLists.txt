cmake_minimum_required(VERSION 3.15)
project(ex3)

set(CMAKE_CXX_STANDARD 14)

add_executable(ex3 main.cpp Command.h OpenServerCommand.cpp OpenServerCommand.h ConnectCommand.h DefineVarCommand.h DefineVarCommand.cpp ConnectCommand.cpp Var.h Var.cpp PrintCommand.cpp PrintCommand.h ConditionParser.cpp ConditionParser.h IfCommand.cpp IfCommand.h LoopCommand.cpp LoopCommand.h)