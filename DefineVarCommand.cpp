//
// Created by nikol on 11/12/2019.
//
#include "DefineVarCommand.h"
#include "Command.h"

 DefineVarCommand::DefineVarCommand(map <string, Var*> * varTableIn) {
     this->varTable = varTableIn;
}
int DefineVarCommand::execute(string * s) {
    Var* newVar = new Var(*s, *(s+ 3), *(s +1));
    this->varTable->insert({*s, newVar});
    return 5;
}