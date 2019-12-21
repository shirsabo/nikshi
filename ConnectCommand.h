//
// Created by nikol on 11/12/2019.
//
//
// Created by nikol on 11/12/2019.
//
#ifndef EX3_CONNECTCOMMAND_H
#define EX3_CONNECTCOMMAND_H

#include "Command.h"

class ConnectCommand : public Command {
private:
    int client_socket = 0;
public:
    int execute(string *);

    void connection(string *s);

    void clientSetter(int socket);

    void changeValue(string sim, double value);
};

#endif //EX3_EX3_CONNECTCOMMAND_H

