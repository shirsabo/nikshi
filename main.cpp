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
#include "SleepCommand.h"
#include <unordered_map>

int sizeAr = 0;
using namespace std;

string *lexer(char *argv[]);

void createMap(unordered_map<string, Command *> *pMap, unordered_map<string, Var *> *varTable,
               unordered_map<string, Var *> *server_map);

void parser(unordered_map<string, Command *> *mp, string *array, int size);

int main(int argsc, char *argv[]) {
    string *array = lexer(argv);
    // creating map for the open server command
    unordered_map<string, Var*> *server_map = new unordered_map<string, Var*>;
    // creating var table
    unordered_map<string, Var *> *varTable = new unordered_map<string, Var*>;
    // creating a map of the commands
    unordered_map<string, Command *> mp;
    createMap(&mp, varTable, server_map);
    for (int i = 0; i < sizeAr; ++i) {
        cout << array[i] << endl;
    }
    parser(&mp, array, sizeAr);
    return 0;
}

void parser(unordered_map<string, Command *> *mp, string *array, int size) {
    int index = 0;
    while (index < size) {
        auto pos = mp->find(array[index]);
        if (pos == mp->end()) {
            // check if it's assignment line (rpm = 0)
        } else {
            Command *c = pos->second;
            string check1 = array[index];
            string check = array[index + 1];
            index += c->execute(&array[index + 1]);
        }
    }
}

void createMap(unordered_map<string, Command *> *pMap, unordered_map<string, Var *> *varTable,
               unordered_map<string, Var *> *server_map) {
    OpenServerCommand *openCommand = new OpenServerCommand(server_map);
    ConnectCommand *connect = new ConnectCommand;
    DefineVarCommand *varCommand = new DefineVarCommand(varTable, server_map);
    PrintCommand *print = new PrintCommand(varTable);
    SleepCommand *sleep = new SleepCommand();
    //must be the last one we enter because it has a map of the commands too
    ConditionParser *I = new IfCommand(pMap, varTable);
    ConditionParser *L = new LoopCommand(pMap, varTable);

    pMap->insert(pair<string, Command *>("openDataServer", openCommand));
    pMap->insert(pair<string, Command *>("connectControlClient", connect));
    pMap->insert(pair<string, Command *>("var", varCommand));
    pMap->insert(pair<string, Command *>("Print", print));
    pMap->insert(pair<string, Command *>("Sleep", sleep));
    pMap->insert(pair<string, Command *>("while", L));
    pMap->insert(pair<string, Command *>("if", I));
}

string *lexer(char *argv[]) {
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
                sizeDeque += 1;
                string sub2 = line.substr(pos - prev + 1, line.length());
                line = sub2;
                prev = 0;
                continue;
            }
            prev = pos + 1;
        }
        if ((line[0] == ' ') || (line[0] == ',') || (line[0] == '(') || (line[0] == ')') || (line[0] == '\t')) {
            line = line.substr(1, line.length());
        }
        // printing the {,} and the end of the line if it does'nt contain any of the checked chars
        if (((line != "" && line != ")") && (line.find_first_of(" ,(\t)", prev)) == std::string::npos)
        || (line.find_first_of("{}", prev)) != std::string::npos) {
            deque.push_back(line);
            sizeDeque += 1;
            continue;
        }
    }
    // entering the deque to an array
    sizeAr = sizeDeque;
    string *array = new string[sizeDeque];
    int i = 0;
    while (sizeDeque > 0) {
        string string = deque.front();
        array[i] = deque.front();
        deque.pop_front();
        i += 1;
        sizeDeque -= 1;
    }
    if (array->length() == 0) {
        return NULL;
    }
    return (array);
}
