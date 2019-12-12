#include "Var.h"
//
#include "DefineVarCommand.h"
#include "Command.h"
#include <string>
Var::Var(string nameIn, string simIn, string inoutIn) {
    this->name = nameIn;
    if (inoutIn == "->") {
        this->inout = "OUT";
    } else if (inoutIn == "<-") {
        this->inout = "IN";
    }
    this->sim = simIn;
}

int Var::getValue() {
    return this->value;
}

