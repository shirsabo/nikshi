//
// Created by nikol on 11/12/2019.
//
//
// Created by nikol on 11/12/2019.
//
#ifndef EX3_CONNECTCOMMAND_H
#define EX3_CONNECTCOMMAND_H

#include "Command.h"
#include "Var.h"
#include <string>
#include <unordered_map>

class ConnectCommand : public Command {
private:
    unordered_map<string, Var *> *varTable;
    int client_socket = 0;
public:
    ConnectCommand(unordered_map<string, Var *> *pMap);

    int execute(string *);

    void connection(string *s);

    void clientSetter(int socket);

    void changeValue(string sim, double value);

    string editSim(string *sim);

    virtual ~ConnectCommand();
};

#endif //EX3_EX3_CONNECTCOMMAND_H

