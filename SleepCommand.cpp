#include "SleepCommand.h"
#include "Command.h"
#include <string>
#include <unistd.h>
#include <chrono>
#include <thread>

//
// Created by nikol on 14/12/2019.
//
int SleepCommand::execute(string *s) {
    string check = *s;
    int sleep_time = stoi(*s);
    this_thread::sleep_for(chrono::milliseconds(sleep_time));
    // jump 2 because of the word sleep and the number
    return 2;
}