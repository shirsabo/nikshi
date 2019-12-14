//
// Created by nikol on 13/12/2019.
//

#include "IfCommand.h"
#include "Command.h"
#include "ConditionParser.h"

IfCommand::IfCommand(map<string, Command *> *mapCommandIn, map<string, Var *> *varTableIn) {
    this->commandMap = mapCommandIn;
    this->varTable = varTableIn;
}

int IfCommand::execute(string *s) {
    return executeHelper(s);
}

IfCommand::~IfCommand() {
}