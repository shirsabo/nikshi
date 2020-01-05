//
// Created by nikol on 13/12/2019.
//

#ifndef EX3_LOOPCOMMAND_H
#define EX3_LOOPCOMMAND_H

#include <map>
#include "Command.h"
#include "ConditionParser.h"
#include <functional>

class LoopCommand : public ConditionParser {
public:
    LoopCommand(unordered_map<string, Command *> *mapCommandIn, unordered_map<string, Var *> *varTableIn,
                mutex *varmutex);

    int execute(string *string);

    ~LoopCommand();

    bool isNumber(string s);
};


#endif //EX3_LOOPCOMMAND_H
