//
// Created by nikol on 11/12/2019.
//
#include "DefineVarCommand.h"
#include "Command.h"
#include <string>
#include "ConditionParser.h"
#include <stdlib.h>

DefineVarCommand::DefineVarCommand(unordered_map<string, Var *> *varTableIn,  unordered_map<string, Var *> *server_mapIn) {
    this->varTable = varTableIn;
    this->server_map = server_mapIn;
}

int DefineVarCommand::execute(string *s) {
    string check = *s;
    if (checkForErrow(s)) {
        // there are -> / <-
        if (Var* var_server = checkInServerMap(s + 3)) {
            // the var exists in the server's map and we will point to it
            this->varTable->insert({*s, var_server});
        } else {
            // there is an errow : ->/<-
            Var *newVar = new Var(*s, *(s + 3), *(s + 1));
            this->varTable->insert({*s, newVar});
        }
        return 5;
    } else if (checkForEqual(s) == 1) {
        // there is a '=' sign
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
            if (!checkIfNumber(*(s+2))) {
                // it's a var and we need to the get it's value
                auto pos = varTable->find(*(s+2));
                if (pos == varTable->end()) {
                    // not a var or a number - error
                    /*** error ***/
                } else {
                    *(s + 2)=  std::to_string(pos->second->getValue());
                }
            }
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

bool DefineVarCommand::checkIfNumber(string s) {
    for (unsigned int i = 0; i < s.length(); i++) {
        if ((i == 0) && (isdigit(s[i]))) {
            continue;
        } else if ((i == 0) && (s[i] == '-')) {
            if ((isdigit(s[1]))) {
                continue;
            } else {
                return false;
            }
        } else if (i == 0) {
            return false;
        }
        if (isdigit(s[i]) == 0 && s[i] != '.' && s[i] != '-') {
            return false;
        }
    }
    return true;
}

Var *DefineVarCommand::checkInServerMap(string *s) {
    string check = *s;
    auto pos = server_map->find(*s);
    if (pos == server_map->end()) {
        return nullptr;
    } else {
        return pos->second;
    }
}
