//
// Created by nikol on 11/12/2019.
//

#ifndef EX3_DEFINEVARCOMMAND_H
#define EX3_DEFINEVARCOMMAND_H

#include "Command.h"
#include "Var.h"
#include <map>

class DefineVarCommand: public Command {
private:
    map <string, Var*>* varTable;
public:
    DefineVarCommand(map <string, Var*>* varTable);
     int execute(string*);
};
#endif //EX3_DEFINEVARCOMMAND_H
