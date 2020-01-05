//
// Created by nikol on 13/12/2019.
//

#ifndef EX3_IFCOMMAND_H
#define EX3_IFCOMMAND_H

#include "ConditionParser.h"
#include "Command.h"
#include <unordered_map>
#include <mutex>

class IfCommand : public ConditionParser {
public:
    IfCommand(unordered_map<string, Command *> *mapCommandIn, unordered_map<string, Var *> *varTableIn,
              mutex *varMutex);

    int execute(string *);

    ~IfCommand();
};


#endif //EX3_IFCOMMAND_H
