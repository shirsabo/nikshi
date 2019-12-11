//
// Created by nikol on 11/12/2019.
//

#ifndef EX3_DEFINEVARCOMMAND_H
#define EX3_DEFINEVARCOMMAND_H

#include "Command.h"

class DefineVarCommand: public Command {
public:
     int execute(string*);
};
#endif //EX3_DEFINEVARCOMMAND_H
