//
// Created by nikol on 13/12/2019.
//

#ifndef EX3_IFCOMMAND_H
#define EX3_IFCOMMAND_H
#include "ConditionParser.h"
#include "Command.h"
#include <map>
class IfCommand: public ConditionParser {
public:
    IfCommand(map<string, Command *> *mapCommandIn, map <string, Var*>* varTableIn);
    int execute(string*) ;
    ~IfCommand();
};


#endif //EX3_IFCOMMAND_H
