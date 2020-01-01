#include "ConditionParser.h"
#include "ShuntingYard.h"

//
// Created by nikol on 13/12/2019.
//
ConditionParser::~ConditionParser() {

}

/** going through the commands between the parenthesis and executing them **/
void ConditionParser::parser(unordered_map<string, Command *> *mp, string *array, int size) {
    int index = 0;
    while (index < size) {
        auto pos = mp->find(array[index]);
        if (pos == mp->end()) {
            index += 1;
        } else {
            Command *c = pos->second;
            string check1 = array[index];
            string check = array[index + 1];
            index += c->execute(&array[index + 1]);
        }
    }
}

/** checking the condition of the Command so we will know if to execute the commands inside the parenthesis **/
bool ConditionParser::checkCondition(string *original) {

    float y = 0, x = 0;
    // calculating using the shunting yard
    x = ShuntingYard::useShuntingYard(original, this->varTable);
    // moving to the sign
    string *sign = original + 1;
    // moving to the next word
    original = original + 2;
    y = ShuntingYard::useShuntingYard(original, this->varTable);
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

/** counting how many places in the array belong to this command so we'll know what number to return
   from the execute func. executing the commands inside the parenthesis accordingly **/
int ConditionParser::executeHelper(string *s) {
    string *original = s;
    string check1 = *original;
    // checking what is the size of the loop command
    // initializing because of the words: {,}
    int sizeLoopCommand = 3, sizeUntilEnd = 0;
    condition = checkCondition(original);
    string shir = *(s + 4);
    while (*s != "{") {
        string check = *s;
        sizeLoopCommand += 1;
        s += 1;
    }
    // moving after the {
    s += 1;
    original = s;
    string check;
    while (*s != "}") {
        check = *s;
        if (*s == "") {
            continue;
        }
        sizeUntilEnd += 1;
        sizeLoopCommand += 1;
        s += 1;
        check = *s;
    }
    // if the condition is false - returning to the main without doing anything
    if (!condition) {
        return sizeLoopCommand;
    }
    // executing the loop if the condition is true
    parser(this->commandMap, original, sizeUntilEnd);
    return sizeLoopCommand;
}


/** getter of the condition - true or false **/
bool ConditionParser::getCondition() {
    return condition;
}


