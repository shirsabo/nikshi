#include "PrintCommand.h"
#include "ShuntingYard.h"
#include <string>
#include <iostream>
#include <stdio.h>
//
// Created by nikol on 12/12/2019.
//
using namespace std;

PrintCommand::PrintCommand(unordered_map <string, Var*> * varTableIn) {
    this->varTable = varTableIn;
}

/** printing a sentence or a number we calculate using the shunting yard **/
int PrintCommand::execute(std::__cxx11::string *print) {
    int index = 2;
    string s = *print;
    // checking if it's a string or a variable
    if ((s).find("\"") == string::npos) {
        float calc = ShuntingYard::useShuntingYard(print, this->varTable);
        printf ("%f\n", calc);
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
        index += 1;
    }
    // printing the last word
    cout << " ";
    string l = (*print).substr(0, (*print).length() - 1);
    cout << (*print).substr(0, (*print).length() - 1)<< endl;
    index += 1;
    return index;
}
