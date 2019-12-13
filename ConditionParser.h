//
// Created by nikol on 13/12/2019.
//

#ifndef EX3_CONDITIONPARSER_H
#define EX3_CONDITIONPARSER_H
#include "Command.h"
#include "Var.h"
//#include "IfCommand.h"
//#include "LoopCommand.h"
#include <map>

class ConditionParser: public Command {
public:
    virtual int execute(string* s) = 0;
    ~ConditionParser();
};
#endif //EX3_CONDITIONPARSER_H
