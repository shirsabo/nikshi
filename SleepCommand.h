//
// Created by nikol on 14/12/2019.
//

#ifndef EX3_SLEEPCOMMAND_H
#define EX3_SLEEPCOMMAND_H

#include "unordered_map"
#include "Var.h"
#include "ShuntingYard.h"
#include "Command.h"
#include "Var.h"
#include <map>

using namespace std;

class SleepCommand : public Command {
private:
    unordered_map<string, Var *> *varTable;
public:
    SleepCommand(unordered_map<string, Var *> *pMap);

    int execute(string *);
};

#endif //EX3_SLEEPCOMMAND_H
