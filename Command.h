//
// Created by nikol on 11/12/2019.
//

#ifndef EX3_COMMAND_H
#define EX3_COMMAND_H

#include <iostream>
#include <string>

using namespace std;

class Command {

public:
    virtual int execute(string *) = 0;

    virtual ~Command() {}
};

#endif //EX3_COMMAND_H
