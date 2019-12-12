//
// Created by nikol on 12/12/2019.
//

#ifndef EX3_PRINTCOMMAND_H
#define EX3_PRINTCOMMAND_H

#include "Command.h"
#include "Var.h"
#include <map>

class PrintCommand: public Command {
private:
    map <string, Var*>* varTable;
public:
    PrintCommand(map <string, Var*>* varTable);
    int execute(string* print);
};

#endif //EX3_PRINTCOMMAND_H
