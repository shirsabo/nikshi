//
// Created by nikol on 11/12/2019.
//

#include "ConnectCommand.h"
#include "Command.h"
#include <sys/socket.h>
#include <string>
#include <iostream>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
using namespace std;
int ConnectCommand::execute(string *s) {
    return 3;
}

void ConnectCommand::connection(string *s) {
    std::cout<<"0"<<endl;
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        //error
        std::cerr << "Could not create a socket" << std::endl;
    }
   std:: cout<<"1"<<endl;
    string local_host = (*s).substr(1, (*s).length() - 2);
    const char *s1 = local_host.c_str();
    //We need to create a sockaddr obj to hold address of server
    sockaddr_in address; //in means IP4
    address.sin_family = AF_INET;//IP4
    address.sin_addr.s_addr = inet_addr(s1);  //the localhost address
    address.sin_port = htons(stoi(*(s + 1)));
    cout<<"2";
    int is_connect = connect(client_socket, (struct sockaddr *) &address, sizeof(address));
    if (is_connect == -1) {
        std::cerr << "Could not connect to host server" << std::endl;
    } else {
        std::cout << "Client is now connected to server" << std::endl;
    }
    std::cout<<"3"<<endl;
    /*
    char buffer[1024] = {0};
    int valread = read( client_socket , buffer, 1024);
    std::cout<<buffer<<std::endl;
     */
    clientSetter(client_socket);
  //  close(client_socket);
    cout << "4"<<endl;
}
void ConnectCommand::changeValue(string sim, double value) {
    //if here we made a connection
    string val_string = to_string(value);
    string send1 = "set " + sim + val_string +"/r/n";
    int is_sent = send(this->client_socket, send1.c_str(), strlen(send1.c_str()), 0);
    if (is_sent == -1) {
        std::cout << "Error sending message" << std::endl;
    } else {
        std::cout << "Hello message sent to server" << std::endl;
    }
}

void ConnectCommand::clientSetter(int socket) {
    this->client_socket = socket;
}