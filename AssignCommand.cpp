//
// Created by oem on 21/12/2019.
//

#include "AssignCommand.h"
#include <string>
#include "Var.h"
#include "ShuntingYard.h"
#include <unordered_map>
#include "ConnectCommand.h"

AssignCommand::AssignCommand(unordered_map<string, Var *> *varTableIn, ConnectCommand *connectIn) {
    this->varTable = varTableIn;
    this->connectCommand = connectIn;
}
int AssignCommand::execute(std::__cxx11::string * s) {
    string check = *(s-2);
    auto pos = varTable->find(*(s-2));
    if (pos == varTable->end()) {
        cout << "error - can't find the val in assignCommand"<<endl;
    } else{
        double val = ShuntingYard::useShuntingYard(s, this->varTable);
        pos->second->setValue(val);
        Var *check2 = pos->second;
        // checking if there is an arrow out and we need to inform the server
        if (pos->second->getArrow() == "OUT") {
            // updating the client server
            connectCommand->changeValue(pos->second->getSim(), val);
        }
    }
    // jump over =, value (no need to jump over the name)
    return 2;
}