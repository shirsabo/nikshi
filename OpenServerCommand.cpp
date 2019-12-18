//
// Created by nikol on 11/12/2019.
//
#include "OpenServerCommand.h"
#include <sys/socket.h>
#include <iostream>
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>
#include <deque>

int OpenServerCommand::execute(string *s) {
    //create socket
    int socketfd = socket(AF_INET, SOCK_STREAM, 0);
    if (socketfd == -1) {
        //error
        std::cerr << "Could not create a socket" << std::endl;
        return -1;
    }
    //bind socket to IP address
    // we first need to create the sockaddr obj.
    sockaddr_in address; //in means IP4
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY; //give me any IP allocated for my machine
    address.sin_port = htons(stoi(*s));
    //we need to convert our number
    // to a number that the network understands.
    //the actual bind command
    if (bind(socketfd, (struct sockaddr *) &address, sizeof(address)) == -1) {
        std::cerr << "Could not bind the socket to an IP" << std::endl;
        return -2;
    }
    //making socket listen to the port
    if (listen(socketfd, 5) == -1) { //can also set to SOMAXCON (max connections)
        std::cerr << "Error during listening command" << std::endl;
        return -3;
    } else {
        std::cout << "Server is now listening ..." << std::endl;
    }
//waiting until connection
    // accepting a client
    int client_socket = accept(socketfd, (struct sockaddr *) &address,
                               (socklen_t *) &address);
    if (client_socket == -1) {
        std::cerr << "Error accepting client" << std::endl;
        return -4;
    }
    close(socketfd); //closing the listening socket
    //reading from client
    char buffer[1024] = {0};
    // updating the var table according to the buffer
    updateMap(buffer, true);

    for (int i = 0; i < 3; i++) {
        int valread = ::read(client_socket, buffer, 1024);
        // updating the var table according to the buffer
        // change the value
        updateMap(buffer, false);
    }
    //writing back to client
    char *hello = "Hello, I can hear you! \n";
    send(client_socket, hello, strlen(hello), 0);
    std::cout << "Hello message sent\n" << std::endl;
    return 2;
}

OpenServerCommand::OpenServerCommand(std::unordered_map<string, Var *> *pMap) {
    this->varTable = pMap;
}

void OpenServerCommand::updateMap(char buffer[1024], bool firstTime) {
    int i = 1;
    string s = buffer;
    size_t prev = 0, pos;
    for (; (pos = s.find_first_of(",", prev)) != std::string::npos; i += 1) {
        if (pos > prev) {
            string sub = s.substr(prev, pos - prev);
            if (firstTime) {
                firstRead(sub, i);
            } else {
                notFirstRead(sub, i);
            }
            string sub2 = s.substr(pos - prev + 1, s.length());
            s = sub2;
            prev = 0;
            continue;
        }
        prev = pos + 1;
    }
/**
    //
    Var *newVar2 = new Var("altimeter_indicated-altitude-ft",
                           "/instrumentation/altimeter/indicated-altitude-ft", "");
    newVar2->setValue(buffer[0]);
    this->varTable->insert({"altimeter_indicated-altitude-ft", newVar2});// altimeter
    Var *newVar2 = new Var("altimeter_indicated-altitude-ft",
                           "/instrumentation/altimeter/indicated-altitude-ft", "");
    newVar2->setValue(buffer[0]);
    this->varTable->insert({"altimeter_indicated-altitude-ft", newVar2});// altimeter
    Var *newVar2 = new Var("altimeter_indicated-altitude-ft",
                           "/instrumentation/altimeter/indicated-altitude-ft", "");
    newVar2->setValue(buffer[0]);
    this->varTable->insert({"altimeter_indicated-altitude-ft", newVar2});// altimeter
    Var *newVar2 = new Var("altimeter_indicated-altitude-ft",
                           "/instrumentation/altimeter/indicated-altitude-ft", "");
    newVar2->setValue(buffer[0]);
    this->varTable->insert({"altimeter_indicated-altitude-ft", newVar2});// altimeter
    Var *newVar2 = new Var("altimeter_indicated-altitude-ft",
                           "/instrumentation/altimeter/indicated-altitude-ft", "");
    newVar2->setValue(buffer[0]);
    this->varTable->insert({"altimeter_indicated-altitude-ft", newVar2});// altimeter
    Var *newVar2 = new Var("altimeter_indicated-altitude-ft",
                           "/instrumentation/altimeter/indicated-altitude-ft", "");
    newVar2->setValue(buffer[0]);
    this->varTable->insert({"altimeter_indicated-altitude-ft", newVar2});// altimeter
    Var *newVar2 = new Var("altimeter_indicated-altitude-ft",
                           "/instrumentation/altimeter/indicated-altitude-ft", "");
    newVar2->setValue(buffer[0]);
    this->varTable->insert({"altimeter_indicated-altitude-ft", newVar2});// altimeter
    Var *newVar2 = new Var("altimeter_indicated-altitude-ft",
                           "/instrumentation/altimeter/indicated-altitude-ft", "");
    newVar2->setValue(buffer[0]);
    this->varTable->insert({"altimeter_indicated-altitude-ft", newVar2});// altimeter
    Var *newVar2 = new Var("altimeter_indicated-altitude-ft",
                           "/instrumentation/altimeter/indicated-altitude-ft", "");
    newVar2->setValue(buffer[0]);
    this->varTable->insert({"altimeter_indicated-altitude-ft", newVar2});// altimeter
    Var *newVar2 = new Var("altimeter_indicated-altitude-ft",
                           "/instrumentation/altimeter/indicated-altitude-ft", "");
    newVar2->setValue(buffer[0]);
    this->varTable->insert({"altimeter_indicated-altitude-ft", newVar2});
    **/
}

ssize_t read(string s, int i) {
    return 0;
}


void OpenServerCommand::firstRead(string sub, int i) {
    if (i == 1) {
        // speed
        Var *newVar1 = new Var("airspeed-indicator_indicated-speed-kt",
                               "/instrumentation/airspeed-indicator/indicated-speed-kt", "<-");
        newVar1->setValue(stof(sub));
        this->varTable->insert({"\"/instrumentation/airspeed-indicator/indicated-speed-kt\"", newVar1});
    } else if (i == 2) {
        // altimeter
        Var *newVar2 = new Var("altimeter_indicated-altitude-ft",
                               "/instrumentation/altimeter/indicated-altitude-ft", "<-");
        newVar2->setValue(stof(sub));
        this->varTable->insert({"\"/instrumentation/altimeter/indicated-altitude-ft\"", newVar2});
    } else if (i == 3) {
        // altimeter
        Var *newVar3 = new Var("altimeter_pressure-alt-ft",
                               "/instrumentation/altimeter/pressure-alt-ft", "<-");
        newVar3->setValue(stof(sub));
        this->varTable->insert({"\"/instrumentation/altimeter/pressure-alt-ft\"", newVar3});
    } else if (i == 4) {
        // attitude
        Var *newVar4 = new Var("attitude-indicator_indicated-pitch-deg",
                               "/instrumentation/attitude-indicator/indicated-pitch-deg", "<-");
        newVar4->setValue(stof(sub));
        this->varTable->insert({"\"/instrumentation/attitude-indicator/indicated-pitch-deg\"", newVar4});
    } else if (i == 5) {
        //
        Var *newVar5 = new Var("attitude-indicator_indicated-roll-deg",
                               "/instrumentation/attitude-indicator/indicated-roll-deg", "<-");
        newVar5->setValue(stof(sub));
        this->varTable->insert({"\"/instrumentation/attitude-indicator/indicated-roll-deg\"", newVar5});
    } else if (i == 6) {
        //
        Var *newVar6 = new Var("attitude-indicator_internal-pitch-deg",
                               "/instrumentation/attitude-indicator/internal-pitch-deg", "<-");
        newVar6->setValue(stof(sub));
        this->varTable->insert({"\"/instrumentation/attitude-indicator/internal-pitch-deg\"", newVar6});
    } else if (i == 7) {
        //
        Var *newVar7 = new Var("attitude-indicator_internal-roll-deg",
                               "/instrumentation/attitude-indicator/internal-roll-deg", "<-");
        newVar7->setValue(stof(sub));
        this->varTable->insert({"\"/instrumentation/attitude-indicator/internal-roll-deg\"", newVar7});
    }

}

void OpenServerCommand::notFirstRead(string s, int i) {

}
