//
// Created by nikol on 13/12/2019.
//
#include "ConditionParser.h"
#include "LoopCommand.h"
#include "Command.h"

LoopCommand::LoopCommand(unordered_map<string, Command *> *mapCommandIn, unordered_map<string, Var *> *varTableIn) {
    this->commandMap = mapCommandIn;
    this->varTable = varTableIn;
}

/** uses the executeHelper func of ConditionParser.
 * if the condition in the loop is false - returning from it the number of steps
 * we need to jump in the array until the end of the parenthesis.
 * if the condition is true - returning 0 so we will execute it again**/
int LoopCommand::execute(string *s) {
    int steps = executeHelper(s);
    if (this->getCondition()) {
        return 0;
    }
    return steps;
}

LoopCommand::~LoopCommand() {
}