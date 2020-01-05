//
// Created by nikol on 11/12/2019.
//
#include "DefineVarCommand.h"
#include "Command.h"
#include <string>
#include <mutex>
#include "ShuntingYard.h"

std::mutex mtx;

DefineVarCommand::DefineVarCommand(unordered_map<string, Var *> *varTableIn, unordered_map<string, Var *> *server_mapIn,
                                   mutex *muteServerMap, mutex *varMute) {
    this->varTable = varTableIn;
    this->server_map = server_mapIn;
    this->mute_s = muteServerMap;
    this->mute_t = varMute;
}

/** creating a new var according to the text file.
 * if the sign is -> we will create a new one,
 * if the sign is <- we will look for it in the map of the vares that were created in the server command
 * if the sign is = we will create it and assign a value to it **/
int DefineVarCommand::execute(string *s) {
    string check = *s;
    string checkSim = *(s + 2);
    //locking var table!
    if (checkForErrow(s)) {
        // there are -> / <-
        Var *var_server;
        /** s+3**/
        if (*(s + 1) == "<-" && (var_server = checkInServerMap(s + 2))) {
            // the var exists in the server's map and we will point to it
            this->varTable->insert({*s, var_server});
        } else {
            // there is an errow : ->
            Var *newVar = new Var(*s, *(s + 2), *(s + 1));
            newVar->setValue(0);
            this->varTable->insert({*s, newVar});
        }
        return 4;
    } else if (checkForEqual(s) == 2) {
        string name = *s;
        string next = *(s + 1);
        double val = ShuntingYard::useShuntingYard((s + 2), this->varTable);
        // x = val
        std::lock_guard<std::mutex> guard(*mute_t);
        auto pos = varTable->find(*s);
        if (pos == varTable->end()) {
            Var *newVar = new Var(name, "", "");
            newVar->setValue(val);
            this->varTable->insert({name, newVar});
        } else {
            pos->second->setValue(val);
        }
        mute_t->unlock();
        return 4;
    }
    // never used
    return 1;
}


/** returns true if the string has ->/<- and false otherwise **/
bool DefineVarCommand::checkForErrow(string *s) {
    string check = *(s + 1);
    if ((*(s + 1) == "->") || (*(s + 1) == "<-")) {
        return true;
    }
    return false;
}

/** returns true if the string has the = sign and false otherwise **/
int DefineVarCommand::checkForEqual(string *s) {
    string check = *s;
    if (((*s).find_first_of("=", 0) != std::string::npos)) {
        return 1;
    } else if ((*(s + 1)).find_first_of("=", 0) != std::string::npos) {
        return 2;
    }
    return 0;
}

/** searching for the var's sim in the map of the var's created in the server command.
 * returning the var if found and nullptr otherwise **/
Var *DefineVarCommand::checkInServerMap(string *s) {
    string check = *s;
    //locking server map!
    std::lock_guard<std::mutex> guard(*mute_s);
    auto pos = server_map->find(*s);
    if (pos == server_map->end()) {
        (*mute_s).unlock();
        return nullptr;
    } else {
        // found in server map
        Var *v = pos->second;
        (*mute_s).unlock();
        return v;
    }
}
