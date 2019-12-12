//
// Created by nikol on 12/12/2019.
//

#ifndef EX3_VAR_H
#define EX3_VAR_H

#include <string>
using namespace std;
class Var {
private:
    int value = 0;
    string sim = "";
    // tells us if we need to output or input the information
    string inout = "";
    string name = "";
public:
    Var(string name, string sim, string inout);
    int getValue();
};
#endif //EX3_VAR_H
