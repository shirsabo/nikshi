//
// Created by nikol on 13/12/2019.
//

#ifndef EX3_IFCOMMAND_H
#define EX3_IFCOMMAND_H
#include "ConditionParser.h"
#include "Command.h"
#include <map>
class IfCommand: public ConditionParser {
private:
    map <string, Var*>* varTable;
public:
    IfCommand(map <string, Var*>* varTableIn);
    int execute(string*) ;
};


#endif //EX3_IFCOMMAND_H
