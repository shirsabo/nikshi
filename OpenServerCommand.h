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

class OpenServerCommand: public Command {
private:
    std::unordered_map<string, Var *> *varTable;
    int *offWhileServer;
    int client_socket = 0;
public:
    OpenServerCommand(std::unordered_map<string, Var *> *pMap, int *offWhileServer);

    virtual int execute(string*);

    void updateMap(char buffer[1024], bool firstTime);

    string initializeVars(string sub, int i, bool firstTime);

    void notFirstRead(string sub, int i);

    int dataEntryPoint(string *s);

    int acceptence(string *s);

    void clientSetter(int socket);
};

#endif //EX3_OPENSERVERCOMMAND_H
