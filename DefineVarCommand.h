//
// Created by nikol on 11/12/2019.
//

#ifndef EX3_DEFINEVARCOMMAND_H
#define EX3_DEFINEVARCOMMAND_H

#include "Command.h"
#include "Var.h"
#include <map>
using namespace std;

class DefineVarCommand: public Command {
private:
    map <string, Var*>* varTable;
public:
    DefineVarCommand(map <string, Var*>* varTable);
     int execute(string*);

    bool checkForErrow(string *pString);

    int checkForEqual(string *pString);

    string* seperateString(string *pString);
};
#endif //EX3_DEFINEVARCOMMAND_H
