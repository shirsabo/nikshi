#include "SleepCommand.h"
#include <string>
#include <chrono>
#include <thread>
#include "ShuntingYard.h"

//
// Created by nikol on 14/12/2019.
//
SleepCommand::SleepCommand(unordered_map<string, Var *> *pMap) {
    this->varTable = pMap;
}

/** calculating the number of seconds we want to sleep by the shunting yard.
 * making the thread sleep that long.**/
int SleepCommand::execute(string *s) {
    string check = *s;
    float calc = ShuntingYard::useShuntingYard(s, this->varTable);
    int sleep_time = (int)(calc);
    this_thread::sleep_for(chrono::milliseconds(sleep_time));
    // jump 2 because of the word sleep and the number
    return 2;
}