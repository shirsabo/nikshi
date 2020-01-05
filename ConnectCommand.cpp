//
// Created by nikol on 11/12/2019.
//

#include "ConnectCommand.h"
#include "ShuntingYard.h"
#include <sys/socket.h>
#include <string>
#include <iostream>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include <unistd.h>

using namespace std;

ConnectCommand::ConnectCommand(unordered_map<string, Var *> *pMap, mutex *varMutexIn) {
    this->varTable = pMap;
    this->varMutex = varMutexIn;
}

/** executing the command - creating a client **/
int ConnectCommand::execute(string *s) {
    string s4 = *s;
    return 2;
}

/** connecting to the client, printing a message if could not connect to host server and exiting **/
void ConnectCommand::connection(string *s) {
    int client_socket1 = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket1 == -1) {
        //error
        cerr << "Could not create a socket" << std::endl;
        exit(1);
    }
    string local_host = s->substr(1, s->find(',') - 2);
    const char *s1 = local_host.c_str();
    string port = s->substr((*s).find(',') + 1, (*s).length());
    //We need to create a sockaddr obj to hold address of server
    sockaddr_in address; //in means IP4
    address.sin_family = AF_INET;//IP4
    address.sin_addr.s_addr = inet_addr(s1);  //the localhost address
    // calculating the local_host
    std::lock_guard<std::mutex> guard(*varMutex);
    float host = ShuntingYard::useShuntingYard(&port, this->varTable);
    varMutex->unlock();
    address.sin_port = htons((int) host);
    int is_connect = connect(client_socket1, (struct sockaddr *) &address, sizeof(address));
    if (is_connect == -1) {
        cerr << "Could not connect to host server" << std::endl;
        exit(1);
    }
    clientSetter(client_socket1);
}

/** changing a var's value in the simulator according to the text file **/
void ConnectCommand::changeValue(string sim, double value) {
    // if here we made a connection
    string val_string = to_string(value);
    sim = editSim(&sim);
    string send1 = "set " + sim + " " + val_string + "\r\n";
    // sending the value to the client so the var's value will change in the simulator
    int is_sent = send(this->client_socket, send1.c_str(), strlen(send1.c_str()), 0);
    if (is_sent == -1) {
        cerr << "Error sending message" << std::endl;
    }
}

/** initializing the client socket fiels in this class **/
void ConnectCommand::clientSetter(int socket) {
    this->client_socket = socket;
}

/** removing the " sign from the sim **/
string ConnectCommand::editSim(string *sim) {
    string check2 = *sim;
    *sim = sim->substr(2, (sim->length() - 3));
    string check = *sim;
    return *sim;
}

/** closing the client socket **/
ConnectCommand::~ConnectCommand() {
    close(this->client_socket);
}
