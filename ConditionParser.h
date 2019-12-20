//
// Created by nikol on 13/12/2019.
//

#ifndef EX3_CONDITIONPARSER_H
#define EX3_CONDITIONPARSER_H
#include "Command.h"
#include "Var.h"
//#include "IfCommand.h"
//#include "LoopCommand.h"
#include <unordered_map>
#include <functional>

class ConditionParser: public Command {
public:
    unordered_map<string, Command *> *commandMap;
    unordered_map <string, Var*>* varTable;

    void parser(unordered_map<string, Command*>* mp, string *array, int size);

    virtual int execute(string* s) = 0;

    ~ConditionParser();

    bool checkCondition(string *original);

    static bool isNumber(string s);

    int executeHelper(string *s);

    double useShuntingYard(string *s);
};
#endif //EX3_CONDITIONPARSER_H
