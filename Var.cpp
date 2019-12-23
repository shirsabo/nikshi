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

string Var::getName() {
    return this->name;
}

float Var::getValue() {
    return this->value;
}

void Var::setValue(float val) {
    this->value = val;
    if (this->getName() == "engine_rpm" && val > 0) {
        cout << "hi";
    }
}

string Var::getArrow(){
    return this->inout;
}

string Var::getSim(){
    return this->sim;
}