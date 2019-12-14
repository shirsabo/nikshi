#include "ConditionParser.h"

//
// Created by nikol on 13/12/2019.
//
ConditionParser::~ConditionParser() {

}

void ConditionParser::parser(map<string, Command *> *mp, string *array, int size) {
    int index = 0;
    while (index < size) {
        auto pos = mp->find(array[index]);
        if (pos == mp->end()) {
            //handle the error
            //throw "bad input";
        } else {
            Command *c = pos->second;
            string check1 = array[index];
            string check = array[index + 1];
            index += c->execute(&array[index + 1]);
        }
    }
}

bool ConditionParser::checkCondition(string *original) {
    double y = 0, x = 0;
    string s = *original;
    // checking if it's a var

    auto pos = ((this->varTable))->find(s);
    if (pos == this->varTable->end()) {
        // checking if it's a number
        if (isNumber(*original)) {
            x = stod(*original);
        } else {
            throw "unvalid input";
        }
    } else {
        // it's a var
        x = pos->second->getValue();
    }
    // moving to the sign
    string *sign = original + 1;
    string check = *sign;
    // moving to the next word
    original = original + 2;
    string num = *original;
    std::string::size_type sz = 0;
    auto pis = ((this->varTable))->find(*original);
    if (pis == this->varTable->end()) {
        // checking if it's a number
        if (isNumber(*original)) {
            y = stod(*original, reinterpret_cast<size_t *>(sz));
        } else {
            throw "unvalid input";
        }
    } else {
        // it's a var
        y = pis->second->getValue();
    }
    if (*sign == "==") {
        if (x == y) {
            return true;
        }
    } else if (*sign == "<=") {
        if (x <= y) {
            return true;
        }
    } else if (*sign == ">=") {
        if (x >= y) {
            return true;
        }
    } else if (*sign == ">") {
        if (x > y) {
            return true;
        }
    } else if (*sign == "<") {
        if (x < y) {
            return true;
        }
    } else if (*sign == "!=") {
        if (x != y) {
            return true;
        }
    } else {
        throw "unvalid input";
    }
    return false;
}


bool ConditionParser::isNumber(string s) {
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

int ConditionParser::executeHelper(string *s) {
    string *original = s;
// checking what is the size of the loop command
// initializing because of the words: {,}
    int sizeLoopCommand = 3, sizeUntilEnd = 0;
    while (*s != "{") {
        string check = *s;
        sizeLoopCommand += 1;
        s += 1;
    }
    bool condition = checkCondition(original);
// moving after the {
    s += 1;
    original = s;
    while (*s != "}") {
        string check = *s;
        sizeUntilEnd += 1;
        sizeLoopCommand += 1;
        s += 1;
    }
// if the condition is false - returning to the main without doing anything
    if (!condition) {
        return sizeLoopCommand;
    }
// executing the loop if the condition is true
    parser(this->commandMap, original, sizeUntilEnd);
    if (typeid(this).name() == "LoppCommand") {
        return 0;
    } else {
        return sizeLoopCommand;
    }
}