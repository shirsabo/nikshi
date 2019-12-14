//
// Created by nikol on 13/12/2019.
//
#include "ConditionParser.h"
#include "LoopCommand.h"
#include "Command.h"
#include <stdlib.h>

LoopCommand::LoopCommand(map<string, Command *> *mapCommandIn, map<string, Var *> *varTableIn) {
    this->commandMap = mapCommandIn;
    this->varTable = varTableIn;
    // this->funcParser = func;
}

int LoopCommand::execute(string *s) {
    return executeHelper(s);
}

LoopCommand::~LoopCommand() {
}