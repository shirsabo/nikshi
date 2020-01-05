//
// Created by oem on 21/12/2019.
//

#ifndef EX3_ASSIGNCOMMAND_H
#define EX3_ASSIGNCOMMAND_H

#include <string>
#include "Var.h"
#include "unordered_map"
#include "Command.h"
#include "ConnectCommand.h"

class AssignCommand : public Command {
private:
    unordered_map<string, Var *> *varTable;
    ConnectCommand *connectCommand;
    mutex *varMutex;
public:
    AssignCommand(unordered_map<string, Var *> *varTable, ConnectCommand *connectIn, mutex *varMutex);

    int execute(string *s);
};

#endif //EX3_ASSIGNCOMMAND_H
