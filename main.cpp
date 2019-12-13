#include <iostream>
#include <fstream>
#include <cstring>
#include <algorithm>
#include <deque>
#include "Command.h"
#include "OpenServerCommand.h"
#include "DefineVarCommand.h"
#include "ConnectCommand.h"
#include "PrintCommand.h"
#include "Var.h"
#include "ConditionParser.h"
#include "LoopCommand.h"
#include "IfCommand.h"
#include <map>

int sizeAr = 0;
using namespace std;

 string *lexer(char *argv[]);

void createMap(map<string, Command*> *pMap, map <string, Var*>* varTable );

int main(int argsc, char *argv[]) {
    string *array = lexer(argv);
    int index = 0;
    // creating var table
    map <string, Var*>* varTable= new map<string, Var*>;
    // creating a map of the commands
    map<string, Command*> mp;
    createMap(&mp, varTable);
    for (int i = 0; i < sizeAr; ++i) {
        cout<<array[i]<<endl;
    }
    while (index < sizeAr) {
        auto pos = mp.find(array[index]);
        if (pos == mp.end()) {
            //handle the error
            //throw "bad input";
        } else {
            Command *c = pos->second;
            string check1 = array[index];
            string check = array[index+1];
            index += c->execute(&array[index+1]);
        }
    }
    return 0;
}
void createMap(map<string, Command*> *pMap, map <string, Var*>* varTable) {
    OpenServerCommand *openCommand = new OpenServerCommand;
    ConnectCommand * connect = new ConnectCommand;
    DefineVarCommand *varCommand = new DefineVarCommand(varTable);
    PrintCommand *print = new PrintCommand(varTable);
    //must be the last one we enter because it has a map of the commands too
    ConditionParser *I = new IfCommand(varTable);
    ConditionParser *L = new LoopCommand(pMap, varTable);
    pMap->insert(pair<string, Command*>("openDataServer", openCommand));
    pMap->insert(pair<string, Command*>("connectControlClient", connect));
    pMap->insert(pair<string, Command*>("var", varCommand));
    pMap->insert(pair<string, Command*>("Print", print));
    pMap->insert(pair<string, Command*>("while", I));
    pMap->insert(pair<string, Command*>("{", L));
}
 string * lexer(char *argv[]) {
    deque<string> deque;
    int sizeDeque = 0;
    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
        printf("can't open the file");
        exit(1);
    }
    string megaString, line;
    ifstream myfile;
    myfile.open(argv[1]);
    //reading the lines in the file
    while (getline(myfile, line)) {
        // creating an array of string from the attached string
        size_t prev = 0, pos;
        while ((pos = line.find_first_of(" ,(\t)", prev)) != std::string::npos) {
            if (pos > prev) {
                string sub1 = line.substr(prev, pos - prev);
                // entering to the deque
                deque.push_back(sub1);
                sizeDeque +=1;
                string sub2 = line.substr(pos - prev + 1, line.length());
                line = sub2;
                prev = 0;
                continue;
            }
            prev = pos + 1;
        }
        // printing the {,} and the end of the line if it does'nt contain any of the checked chars
        if (((line != "") && (line.find_first_of(" ,(\t)", prev)) == std::string::npos &&
        (line.find_first_of("{}", prev)) != std::string::npos)) {
            deque.push_back(line);
            sizeDeque +=1;
            continue;
        }
    }
    // entering the vector to an array
    sizeAr = sizeDeque;
    string *array = new string[sizeDeque];
    int i = 0;
    while (sizeDeque > 0) {
        string string = deque.front();
        array[i] = deque.front();
        //cout<<array[i]<<endl;
        deque.pop_front();
        i += 1;
        sizeDeque -=1;

    }
    if (array->length() == 0) {
        return NULL;
    }
    return (array);
}
