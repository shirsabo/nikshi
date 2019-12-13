//
// Created by nikol on 13/12/2019.
//

#ifndef EX3_LOOPCOMMAND_H
#define EX3_LOOPCOMMAND_H

#include <map>
#include "Command.h"
#include "ConditionParser.h"

class LoopCommand : public ConditionParser {
private:
    // <number of command, Command>
    // helps us keep the order of the commands in the loop
    map<int, Command *> *numberCommands;
    map<string, Command *> *commandMap;
    map <string, Var*>* varTable;
public:
    LoopCommand(map<string, Command *> *mapCommandIn, map <string, Var*>* varTableIn);
    int execute(string *string);
};


#endif //EX3_LOOPCOMMAND_H
