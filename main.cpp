#include <iostream>
#include <fstream>
#include <cstring>
#include <algorithm>
#include <deque>
#include "Command.h"
#include "OpenServerCommand.h"
#include "DefineVarCommand.h"
#include "ConnectCommand.h"
#include <map>

using namespace std;

string *lexer(char *argv[]);

//void createMap(map<std::__cxx11::string, struct Command> *map);

void createMap(map<string, Command*> *pMap);

int main(int argsc, char *argv[]) {
    string *array = lexer(argv);
    int index = 0;
    // creating a map of the commands
    map<string, Command*> mp;
    createMap(&mp);
    while (index < array->size()) {
        auto pos = mp.find(array[index]);
        if (pos == mp.end()) {
            //handle the error
            //c = NULL;
        } else {
            Command *c = pos->second;
            index += c->execute(&array[index]);
        }
    }
    return 0;
}

void createMap(map<string, Command*> *pMap) {
    OpenServerCommand *openCommand = new OpenServerCommand;
    ConnectCommand * connect = new ConnectCommand;
    DefineVarCommand *varCommand = new DefineVarCommand;
    //DefineVarCommand * var = new
    pMap->insert(pair<string, Command*>("openDataServer", openCommand));
    pMap->insert(pair<string, Command*>("connectControlClient", connect));
    pMap->insert(pair<string, Command*>("var", varCommand));
}
string *lexer(char *argv[]) {
    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
        printf("can't open the file");
        exit(1);
    }
    string megaString;
    string line;
    ifstream myfile;
    myfile.open(argv[1]);
    //entering the strings to a vector
    deque<string> deque;
    //reading the lines in the file
    while (getline(myfile, line)) {
        // creating an array of string from the attached string
        size_t prev = 0, pos;
        while ((pos = line.find_first_of(" ,()", prev)) != std::string::npos) {
            if (pos > prev) {
                string sub1 = line.substr(prev, pos - prev);
                // entering to the vector
                deque.push_back(sub1);
                string sub2 = line.substr(pos - prev + 1, line.length());
                line = sub2;
                prev = 0;
                continue;
            }
            prev = pos + 1;
        }
    }
    // entering the vector to an array
    const int s = deque.size();
    string *array = new string[s];
    int i = 0;
    while (deque.size() > 0) {
        array[i] = deque.front();
        deque.pop_front();
        i += 1;
    }
    if (array->length() == 0) {
        return NULL;
    }
    return (array);
}
