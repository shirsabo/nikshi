//
// Created by nikol on 13/12/2019.
//

#include "IfCommand.h"
#include "Command.h"
#include "ConditionParser.h"
IfCommand::IfCommand (map <string, Var*>* varTableIn) {
    this->varTable = varTableIn;
}
int IfCommand::execute(string *s) {
    // checking what is the size of the if command
    // initialize 1 because of the word: while
    int sizeIfCommand = 1;
    while (*s != "{") {
        string check = *s;
        sizeIfCommand += 1;
        s += 1;
    }
    return sizeIfCommand;
}