//
// Created by nikol on 13/12/2019.
//

#include "IfCommand.h"
#include "Command.h"
#include "ConditionParser.h"

IfCommand::IfCommand(unordered_map<string, Command *> *mapCommandIn, unordered_map<string, Var *> *varTableIn) {
    this->commandMap = mapCommandIn;
    this->varTable = varTableIn;
}

/** uses the executeHelper func of ConditionParser, returning from it the number of steps
 * we need to jump in the array until the end of the parenthesis **/
int IfCommand::execute(string *s) {
    return executeHelper(s);
}

IfCommand::~IfCommand() {
}