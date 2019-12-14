//
// Created by nikol on 14/12/2019.
//

#ifndef EX3_SLEEPCOMMAND_H
#define EX3_SLEEPCOMMAND_H

#include "Command.h"
#include "Var.h"
#include <map>

using namespace std;

class SleepCommand : public Command {
public:
    int execute(string *);
};

#endif //EX3_SLEEPCOMMAND_H
