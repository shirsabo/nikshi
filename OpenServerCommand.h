//
// Created by nikol on 11/12/2019.
//

#ifndef EX3_OPENSERVERCOMMAND_H
#define EX3_OPENSERVERCOMMAND_H

#include "Command.h"

class OpenServerCommand: public Command {
public:
    virtual int execute(string*);
};

#endif //EX3_OPENSERVERCOMMAND_H
