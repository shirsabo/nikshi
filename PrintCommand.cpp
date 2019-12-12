#include "PrintCommand.h"
#include <string>
#include <iostream>
#include <stdio.h>
//
// Created by nikol on 12/12/2019.
//
using namespace std;

PrintCommand::PrintCommand(map <string, Var*> * varTableIn) {
    this->varTable = varTableIn;
}
int PrintCommand::execute(std::__cxx11::string *print) {
    int index = 2;
    string s = *print;
    // checking if it's a string or a variable
    if ((s).find("\"") == string::npos) {
        // finding the value of the veriable
        auto find = varTable->find(s);
        if (find == varTable->end()) {
            //handle the error
            //throw "bad input";
        } else {
            int val = find->second->getValue();
            printf ("%d\n", val);
        }
        return index;
    }
    //printing the first word
    string m = (*print).substr(1, (*print).size());
    if ((m).find("\"") != string::npos) {
        cout << (m).substr(0, (m).length() - 1)<< endl;
        return index;
    }
    std::cout << m;
    printf ("%s", "");
    print = print + 1;
    while ((*(print)).find("\"") == string::npos) {
        std::cout << " ";
        std::cout << *print;
        print = print + 1;
        string s = *print;
        index += 1;
    }
    // printing the last word
    cout << " ";
    string l = (*print).substr(0, (*print).length() - 1);
    cout << (*print).substr(0, (*print).length() - 1)<< endl;
    index += 1;
    return index;
}
