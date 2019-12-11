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
     int execute(string *) {
         return 0;
     };
     ~Command() {}
};
#endif //EX3_COMMAND_H
