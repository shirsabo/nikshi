//
// Created by nikol on 12/12/2019.
//

#ifndef EX3_PRINTCOMMAND_H
#define EX3_PRINTCOMMAND_H

#include "Command.h"
#include "Var.h"
#include <unordered_map>
#include <mutex>

class PrintCommand: public Command {
private:
    unordered_map <string, Var*>* varTable;
    mutex* varMute;
public:
    PrintCommand(unordered_map <string, Var*>* varTable,mutex* varMute);

    int execute(string* print);
};

#endif //EX3_PRINTCOMMAND_H
