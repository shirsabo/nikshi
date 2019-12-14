//
// Created by nikol on 11/12/2019.
//
#include "DefineVarCommand.h"
#include "Command.h"
#include <string>
#include <stdlib.h>

DefineVarCommand::DefineVarCommand(map<string, Var *> *varTableIn) {
    this->varTable = varTableIn;
}

int DefineVarCommand::execute(string *s) {
    string check = *s;
    if (checkForErrow(s)) {
        // there is an erros : ->/<-
        Var *newVar = new Var(*s, *(s + 3), *(s + 1));
        this->varTable->insert({*s, newVar});
        return 5;
    } else if (checkForEqual(s) == 1) {
        string *arr = seperateString(s);
        if (arr[1] != "") {
            // x=val
            auto pos = varTable->find(arr[0]);
            if (pos == varTable->end()) {
                Var *newVar = new Var(arr[0], "", "");
                newVar->setValue(stod(arr[1]));
                this->varTable->insert({arr[0], newVar});
            } else {
                pos->second->setValue(stod(arr[1]));
            }
            return 2;
        } else {
            // x= val
            auto pos = varTable->find(arr[0]);
            if (pos == varTable->end()) {
                Var *newVar = new Var(arr[0], "", "");
                newVar->setValue(stod(*(s + 1)));
                this->varTable->insert({arr[0], newVar});
            } else {
                pos->second->setValue(stod(*(s + 1)));
            }
            return 3;
        }
    } else if (checkForEqual(s) == 2) {
        string name = *s;
        string next = *(s+1);
        if (*(s+1) == "=") {
            // x = val
            auto pos = varTable->find(*s);
            if (pos == varTable->end()) {
                Var *newVar = new Var(name, "", "");
                string num = *(s + 2);
                newVar->setValue(stod(*(s + 2)));
                this->varTable->insert({name, newVar});
            } else {
                pos->second->setValue(stod(*(s + 2)));
            }
            return 4;
        } else {
            // x =val
            std::size_t index = (*(s + 1)).find_first_of("=", 0);
            string val = (*(s + 1)).substr(index + 1, (*(s + 1)).length());
            auto pos = varTable->find(*s);
            if (pos == varTable->end()) {
                Var *newVar = new Var(name, "", "");
                newVar->setValue(stod(val));
                this->varTable->insert({name, newVar});
            } else {
                pos->second->setValue(stod(val));
            }
            return 3;
        }
    }
}

bool DefineVarCommand::checkForErrow(string *s) {
    string check = *(s + 1);
    if ((*(s + 1) == "->") || (*(s + 1) == "<-")) {
        return true;
    }
    return false;
}

int DefineVarCommand::checkForEqual(string *s) {
    string check = *s;
    if (((*s).find_first_of("=", 0) != -1)) {
        return 1;
    } else if ((*(s+1)).find_first_of("=", 0) != -1){
        return 2;
    }
    return 0;
}

string *DefineVarCommand::seperateString(string *s) {
    string check = *s;
    string *arr = new string[2];
    std::size_t index = (*s).find_first_of("=", 0);
    string sub1 = (*s).substr(0, index);
    arr[0] = sub1;
    string sub2 = (*s).substr(index + 1, (*s).length());
    if (sub2 != "") {
        arr[1] = sub2;
    } else {
        arr[1] = "";
    }
    return arr;
}
