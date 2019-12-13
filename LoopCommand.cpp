//
// Created by nikol on 13/12/2019.
//
#include "ConditionParser.h"
#include "LoopCommand.h"
#include "Command.h"
LoopCommand::LoopCommand(map<string, Command *> *mapCommandIn, map <string, Var*>* varTableIn) {
    this->commandMap = mapCommandIn;
    this->varTable = varTableIn;
}

int LoopCommand::execute(string *s) {
    // checking what is the size of the loop command
    // initializing because of the words: {,}
    int sizeLoopCommand = 2;
    // moving after the {
    s += 1;
    while (*s != "}") {
        string check = *s;
        sizeLoopCommand += 1;
        s += 1;
    }
    return sizeLoopCommand;
}