//
// Created by nikol on 11/12/2019.
//
#include "OpenServerCommand.h"
#include <sys/socket.h>
#include <iostream>
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>

  int OpenServerCommand::execute(string *s) {
      //create socket
      int socketfd = socket(AF_INET, SOCK_STREAM, 0);
      if (socketfd == -1) {
          //error
          std::cerr << "Could not create a socket"<<std::endl;
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
          std::cerr<<"Could not bind the socket to an IP"<<std::endl;
          return -2;
      }
      //making socket listen to the port
      if (listen(socketfd, 5) == -1) { //can also set to SOMAXCON (max connections)
          std::cerr<<"Error during listening command"<<std::endl;
          return -3;
      } else{
          std::cout<<"Server is now listening ..."<<std::endl;
      }
//waiting until connection
      // accepting a client
      int client_socket = accept(socketfd, (struct sockaddr *)&address,
                                 (socklen_t*)&address);
      if (client_socket == -1) {
          std::cerr<<"Error accepting client"<<std::endl;
          return -4;
      }
      close(socketfd); //closing the listening socket
      //reading from client
      char buffer[1024] = {0};
      // updating the var table according to the buffer
      updateMap(buffer);
      //valread - bits
      int valread = read( client_socket , buffer, 1024);
      std::cout<<buffer<<std::endl;
      //writing back to client
      char *hello = "Hello, I can hear you! \n";
      send(client_socket , hello , strlen(hello) , 0 );
      std::cout<<"Hello message sent\n"<<std::endl;
      return 2;
}

OpenServerCommand::OpenServerCommand(std::unordered_map<string, Var *> *pMap) {
    this->varTable = pMap;
}

void OpenServerCommand::updateMap(char *buffer) {
    // speed
    Var *newVar1 = new Var("airspeed-indicator_indicated-speed-kt",
            "/instrumentation/airspeed-indicator/indicated-speed-kt", "");
    newVar1->setValue(buffer[0]);
    this->varTable->insert({"/instrumentation/airspeed-indicator/indicated-speed-kt", newVar1});

    // altimeter
    Var *newVar2 = new Var("altimeter_indicated-altitude-ft",
                          "/instrumentation/altimeter/indicated-altitude-ft", "");
    newVar2->setValue(buffer[1]);
    this->varTable->insert({"/instrumentation/altimeter/indicated-altitude-ft", newVar2});

    // altimeter
    Var *newVar3 = new Var("altimeter_pressure-alt-ft",
                           "/instrumentation/altimeter/pressure-alt-ft", "");
    newVar3->setValue(buffer[2]);
    this->varTable->insert({"/instrumentation/altimeter/pressure-alt-ft", newVar3});

    // attitude
    Var *newVar4 = new Var("attitude-indicator_indicated-pitch-deg",
                           "/instrumentation/attitude-indicator/indicated-pitch-deg", "");
    newVar4->setValue(buffer[3]);
    this->varTable->insert({"/instrumentation/attitude-indicator/indicated-pitch-deg", newVar4});

    // altimeter
    Var *newVar5 = new Var("attitude-indicator_indicated-roll-deg",
                           "/instrumentation/attitude-indicator/indicated-roll-deg", "");
    newVar2->setValue(buffer[4]);
    this->varTable->insert({"/instrumentation/attitude-indicator/indicated-roll-deg", newVar5});

    //
    Var *newVar6 = new Var("attitude-indicator_internal-pitch-deg",
                           "/instrumentation/attitude-indicator/internal-pitch-deg", "");
    newVar2->setValue(buffer[5]);
    this->varTable->insert({"/instrumentation/attitude-indicator/internal-pitch-deg", newVar6});

    //
    Var *newVar7 = new Var("attitude-indicator_internal-roll-deg",
                           "/instrumentation/attitude-indicator/internal-roll-deg", "");
    newVar2->setValue(buffer[6]);
    this->varTable->insert({"/instrumentation/attitude-indicator/internal-roll-deg", newVar7});
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
