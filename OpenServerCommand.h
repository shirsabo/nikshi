//
// Created by nikol on 11/12/2019.
//

#ifndef EX3_OPENSERVERCOMMAND_H
#define EX3_OPENSERVERCOMMAND_H

#include <unordered_map>
#include <bits/unordered_map.h>
#include <map>
#include "Command.h"
#include "Var.h"
#include <deque>
#include <mutex>

class OpenServerCommand: public Command {
private:
    unordered_map<string, Var *> *varTable;
    int *offWhileServer;
    int client_socket;
    mutex* muteServer;
public:
    OpenServerCommand(std::unordered_map<string, Var *> *pMap, int *offWhileServer,mutex* muteServerMap);

    virtual int execute(string*);

    void updateMap(string buffer, bool firstTime);

    string initializeVars(string sub, int i, bool firstTime);

    void notFirstRead(string sub, int i);

    int dataEntryPoint();

    void acceptence(string *s);

    void clientSetter(int socket);

    void initializeServerMap();

    string readOneChar();
};

#endif //EX3_OPENSERVERCOMMAND_H
