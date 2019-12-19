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
#include <thread>
#include "ShuntingYard.h"

int sizeAr = 0;
using namespace std;

string *lexer(char *argv[]);

void createMap(unordered_map<string, Command *> *pMap, unordered_map<string, Var *> *varTable,
               unordered_map<string, Var *> *server_map, int *offWhileServer);

void parser(unordered_map<string, Command *> *mp, string *array, int size, int *offWhileServer);

int main(int argsc, char *argv[]) {
    int* offWhileServer;
    int off = 0;
    offWhileServer = & off;
    string *array = lexer(argv);
    // creating map for the open server command
    unordered_map<string, Var *> *server_map = new unordered_map<string, Var *>;
    // creating var table
    unordered_map<string, Var *> *varTable = new unordered_map<string, Var *>;
    /*
    Var *va = new Var("s", "","");
    va->setValue(5);
    varTable->insert({"s", va});
    Interpreter* interpret = new Interpreter();
    interpret->setVariables(varTable);
     */

    // creating a map of the commands
    unordered_map<string, Command *> mp;
    createMap(&mp, varTable, server_map, offWhileServer);
    for (int i = 0; i < sizeAr; ++i) {
        cout << array[i] << endl;
    }
    parser(&mp, array, sizeAr, offWhileServer);
    return 0;
}

void parser(unordered_map<string, Command *> *mp, string *array, int size, int *offWhileServer) {
    int index = 0;

    // executing the first two lines
    auto pos = mp->find(array[index]);
    if (pos == mp->end()) {
        // check if it's assignment line (rpm = 0)
        /*** error ***/
    }
    Command *c = pos->second;
    OpenServerCommand c1 = *((OpenServerCommand *) c);
    // waiting for the server to accept the call
    thread t1(&OpenServerCommand::acceptence, ref((c1)), &(array[index + 1]));
    t1.join();
    // strting to get information from the server
    thread t2(&OpenServerCommand::dataEntryPoint, ref((c1)), &(array[index + 1]));

    while (index < size) {
        auto pos = mp->find(array[index]);
        if (pos == mp->end()) {
            // check if it's assignment line (rpm = 0)
            /*** error ***/
            index+=1;
        } else {
            Command *c = pos->second;
            string check1 = array[index];
            string check = array[index + 1];
            index += c->execute(&array[index + 1]);
        }
    }
    // ending the loop in the open server command
    *offWhileServer = 1;
    t2.join();
}

void createMap(unordered_map<string, Command *> *pMap, unordered_map<string, Var *> *varTable,
        unordered_map<string, Var *> *server_map, int *offWhileServer) {
    OpenServerCommand *openCommand = new OpenServerCommand(server_map, offWhileServer);
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
    pMap->insert(pair<string, Command *>("print", print));
    pMap->insert(pair<string, Command *>("sleep", sleep));
    pMap->insert(pair<string, Command *>("While", L));
    pMap->insert(pair<string, Command *>("If", I));
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
        while ((pos = line.find_first_of(" ,(\t)=", prev)) != std::string::npos) {
            if (pos > prev) {
                string sub1 = line.substr(prev, pos - prev);
                if (sub1 =="While" || sub1 == "while") {
                    cout<< "2";
                }
                // entering to the deque
                deque.push_back(sub1);
                sizeDeque += 1;
                string sub2 = line.substr(pos - prev + 1, line.length());
                line = sub2;
                prev = 0;
                continue;
            }
            if (line[pos] == '=') {
                //string sub1 = line.substr(prev, pos - prev);
                //deque.push_back(sub1);
                deque.push_back("=");
                string sub2 = line.substr(pos - prev + 1, line.length());
                deque.push_back(sub2);
                sizeDeque += 3;
                prev = 0;
                break;
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

