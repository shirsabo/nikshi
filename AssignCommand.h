//
// Created by oem on 21/12/2019.
//

#ifndef EX3_ASSIGNCOMMAND_H
#define EX3_ASSIGNCOMMAND_H

#include <string>
#include "Var.h"
#include "unordered_map"
#include "Command.h"

class AssignCommand: public Command {
private:
    unordered_map<string, Var *> *varTable;
public:
    AssignCommand(unordered_map<string, Var *> *varTableIn);

    int execute(string *s);
};

#endif //EX3_ASSIGNCOMMAND_H
