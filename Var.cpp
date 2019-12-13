#include "Var.h"
#include "DefineVarCommand.h"
#include "Command.h"
#include <string>

Var::Var(string nameIn, string simIn, string inoutIn) {
    this->name = nameIn;
    if (inoutIn == "->") {
        this->inout = "OUT";
    } else if (inoutIn == "<-") {
        this->inout = "IN";
    } else {
        // in cases we just want to create a variable and initialize it
        this->inout = "";
    }
    this->sim = simIn;
}

int Var::getValue() {
    return this->value;
}

void Var::setValue(int val) {
    this->value = val;
}
