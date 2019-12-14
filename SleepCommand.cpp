#include "SleepCommand.h"
#include "Command.h"
#include <string>
#include <unistd.h>

//
// Created by nikol on 14/12/2019.
//
int SleepCommand::execute(string *s) {
    string check = *s;
    usleep(stoi(*s));
    // jump 2 because of the word sleep and the number
    return 2;
}