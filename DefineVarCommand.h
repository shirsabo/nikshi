//
// Created by nikol on 11/12/2019.
//

#ifndef EX3_DEFINEVARCOMMAND_H
#define EX3_DEFINEVARCOMMAND_H

#include "Command.h"
#include "Var.h"
#include <unordered_map>
using namespace std;

class DefineVarCommand: public Command {
private:
    unordered_map <string, Var*>* varTable;
    unordered_map<string, Var *> *server_map;
public:
    DefineVarCommand(unordered_map <string, Var*>* varTable,  unordered_map<string, Var *> *server_map);
     int execute(string*);

    bool checkForErrow(string *pString);

    int checkForEqual(string *pString);

    string* seperateString(string *pString);

    bool checkIfNumber(string s);

    Var *checkInServerMap(string *pString);
};
#endif //EX3_DEFINEVARCOMMAND_H
