//
// Created by nikol on 14/12/2019.
//
/*
#include "Client.h"

int main(int argc, char const *argv[])
{
       int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        //error
        std::cerr << "Could not create a socket" << std::endl;
    }
    string local_host = (*s).substr(1, (*s).length() - 1);
    const char *s1 = local_host.c_str();
    //We need to create a sockaddr obj to hold address of server
    sockaddr_in address; //in means IP4
    address.sin_family = AF_INET;//IP4
    address.sin_addr.s_addr = inet_addr(s1);  //the localhost address
    address.sin_port = htons(stoi(*(s + 1)));
    int is_connect = connect(client_socket, (struct sockaddr *) &address, sizeof(address));
    if (is_connect == -1) {
        std::cerr << "Could not connect to host server" << std::endl;
    } else {
        std::cout << "Client is now connected to server" << std::endl;
    }
    /*
    char buffer[1024] = {0};
    int valread = read( client_socket , buffer, 1024);
    std::cout<<buffer<<std::endl;

clientSetter(client_socket);
close(client_socket);
}
 */