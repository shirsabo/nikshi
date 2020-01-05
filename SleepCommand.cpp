#include "SleepCommand.h"
#include <string>
#include <chrono>
#include <thread>
#include <mutex>
#include "ShuntingYard.h"

//
// Created by nikol on 14/12/2019.
//
SleepCommand::SleepCommand(unordered_map<string, Var *> *pMap, mutex *varMuteIn) {
    this->varTable = pMap;
    this->varMute = varMuteIn;
}

/** calculating the number of seconds we want to sleep by the shunting yard.
 * making the thread sleep that long.**/
int SleepCommand::execute(string *s) {
    string check = *s;
    std::lock_guard<std::mutex> guard(*varMute);
    float calc = ShuntingYard::useShuntingYard(s, this->varTable);
    varMute->unlock();
    int sleep_time = (int) (calc);
    this_thread::sleep_for(chrono::milliseconds(sleep_time));
    // jump 2 because of the word sleep and the number
    return 2;
}