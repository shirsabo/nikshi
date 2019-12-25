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
#include "AssignCommand.h"

int sizeAr = 0;
using namespace std;

string *lexer(char *argv[]);

void createMap(unordered_map<string, Command *> *pMap, unordered_map<string, Var *> *varTable,
               unordered_map<string, Var *> *server_map, int *offWhileServer);

void parser(unordered_map<string, Command *> *mp, string *array, int size, int *offWhileServer);

string edit(string s);

void editConditionParser(string s, deque<string> *deque, int *sizeDeque);

bool isSign(char &c);

string *buildArr(int sizeDeque, deque<string> deque);

void iterateParser(int size, unordered_map<string, Command *> *mp, int *index, string *array);

void deleteProject(unordered_map<string, Var *> *serverMap, unordered_map<string, Var *> *varTable,
                   unordered_map<string, Command *> *mp);

void enterLine(char token, deque<string> *deque, size_t pos, size_t *prev, string line, int *sizeDeque);

void deleteCommands(deque<Command *> deque);

int main(int argsc, char *argv[]) {
    if (argsc != 2) {
        cout << "unexpected arguments" << endl;
    }
    int *offWhileServer;
    int off = 0;
    offWhileServer = &off;
    string *array = lexer(argv);
    // creating map for the open server command
    unordered_map<string, Var *> *server_map = new unordered_map<string, Var *>;
    // creating var table
    unordered_map<string, Var *> *varTable = new unordered_map<string, Var *>;
    // creating a map of the commands
    unordered_map<string, Command *> mp;
    createMap(&mp, varTable, server_map, offWhileServer);
    parser(&mp, array, sizeAr, offWhileServer);
    deleteProject(server_map, varTable, &mp);

    return 0;
}

void deleteProject(unordered_map<string, Var *> *serverMap, unordered_map<string, Var *> *varTable,
                   unordered_map<string, Command *> *mp) {
    for (auto &iter:*serverMap) {
        delete iter.second;
    }
    for (auto &iter:*varTable) {
        delete iter.second;
    }
    for (auto &iter:*mp) {
        delete iter.second;
    }
}

void parser(unordered_map<string, Command *> *mp, string *array, int size, int *offWhileServer) {
    int index = 0;
    thread t3;
    thread t4;
    Command *c, *m;
    OpenServerCommand *c1;
    ConnectCommand *m2;
    // executing the first two lines
    for (int i = 0; i < 2; i++) {
        auto pos = mp->find(array[index]);
        if (pos == mp->end()) {
            // check if it's assignment line (rpm = 0)
            cout << "error in parser\n";
        } else {
            if (i == 0) {
                c = pos->second;
                c1 = ((OpenServerCommand *) c);
                // waiting for the server to accept the call
                thread t1(&OpenServerCommand::acceptence, ref((c1)), &(array[index + 1]));
                t1.join();
                thread t2(&OpenServerCommand::initializeServerMap, ref((c1)));
                t2.join();
                // starting to get information from the server
                t3 = thread(&OpenServerCommand::dataEntryPoint, ref((c1)));
                index += 2;
                continue;
            }
            m = pos->second;
            // opening the clint server
            m2 = ((ConnectCommand *) m);
            string check = array[index + 1];
            t4 = thread(&ConnectCommand::connection, ref(m2), &array[index + 1]);
            t4.join();
        }
    }
    // iterating over the array
    iterateParser(size, mp, &index, array);
    // ending the loop in the open server command
    *offWhileServer = 1;
    t3.join();
}

void iterateParser(int size, unordered_map<string, Command *> *mp, int *index, string *array) {
    //deque<Command *> dequeCommand;
    while (*index < size) {
        auto pos = mp->find(array[*index]);
        if (pos == mp->end()) {
            // check if it's assignment line (rpm = 0)
            *index += 1;
        } else {
            Command *c = pos->second;
            string check1 = array[*index];
            string check = array[*index + 1];
            *index += c->execute(&array[*index + 1]);
            //dequeCommand.push_front(c);
        }
    }
    // delete all the commands after they were executed
    //deleteCommands(dequeCommand);
}

void deleteCommands(deque<Command *> deque) {
    while (deque.size() > 0) {
        Command *c = deque.front();
        deque.pop_front();
        delete c;
    }
}

void createMap(unordered_map<string, Command *> *pMap, unordered_map<string, Var *> *varTable,
               unordered_map<string, Var *> *server_map, int *offWhileServer) {
    OpenServerCommand *openCommand = new OpenServerCommand(server_map, offWhileServer);
    ConnectCommand *connect = new ConnectCommand(varTable);
    DefineVarCommand *varCommand = new DefineVarCommand(varTable, server_map);
    PrintCommand *print = new PrintCommand(varTable);
    SleepCommand *sleep = new SleepCommand(varTable);
    AssignCommand *assign = new AssignCommand(varTable, connect);
    //must be the last one we enter because it has a map of the commands too
    ConditionParser *ifCommand = new IfCommand(pMap, varTable);
    ConditionParser *loopCommand = new LoopCommand(pMap, varTable);

    pMap->insert(pair<string, Command *>("openDataServer", openCommand));
    pMap->insert(pair<string, Command *>("connectControlClient", connect));
    pMap->insert(pair<string, Command *>("var", varCommand));
    pMap->insert(pair<string, Command *>("Print", print));
    pMap->insert(pair<string, Command *>("Sleep", sleep));
    pMap->insert(pair<string, Command *>("while", loopCommand));
    pMap->insert(pair<string, Command *>("if", ifCommand));
    pMap->insert(pair<string, Command *>("=", assign));
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
                // entering to the deque
                deque.push_back(sub1);
                sizeDeque += 1;
                string sub2 = line.substr(pos - prev + 1, line.length());
                if (line[pos] == '=') {
                    enterLine('=', &deque, pos, &prev, line, &sizeDeque);
                    break;
                } else if (line[pos] == '(') {
                    // checking that it's not a sentence in Print
                    if ((deque.back() == "Print" && line.find("\"") != std::string::npos)) {
                        // do nothing
                    } else {
                        enterLine('(', &deque, pos, &prev, line, &sizeDeque);
                        break;
                    }
                }
                if (sub1 == "while" || sub1 == "if") {
                    editConditionParser(sub2, &deque, &sizeDeque);
                    prev = 0;
                    line = "";
                    break;
                }
                line = sub2;
                prev = 0;
                continue;
            }
            if (line[pos] == '=') {
                if (pos == 0) {
                    enterLine('=', &deque, pos, &prev, line, &sizeDeque);
                    break;
                } else {
                    string sub2 = line.substr(pos - prev + 1, line.length());
                    line = sub2;
                    prev = 0;
                    continue;
                }
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
            line = "";
            continue;
        }
    }
    // entering the deque to an array
    string *array = buildArr(sizeDeque, deque);
    if (array->length() == 0) {
        return NULL;
    }
    return array;
}

void enterLine(char token, deque <string> *deque, size_t pos, size_t *prev, string line, int *sizeDeque) {
    string sub2;
    if (token == '=') {
        deque->push_back("=");
        *sizeDeque += 1;
    }
    if (pos > 0) {
        sub2 = line.substr(pos + 1, line.length());
    } else {
        sub2 = line.substr(pos - *prev + 1, line.length());
    }
    sub2 = edit(sub2);
    if (token == '(') {
        // cutting the ')' at the end
        sub2 = sub2.substr(0, sub2.length() - 1);
    }
    (*deque).push_back(sub2);
    *sizeDeque += 1;
    *prev = 0;
}

string *buildArr(int sizeDeque, deque <string> deque) {
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
    return array;
}

void editConditionParser(string s, deque <string> *deque, int *sizeDeque) {
    size_t prev = 0, pos;
    string afterSign = s;
    if ((pos = s.find_first_of("<>=!", prev)) != std::string::npos) {
        string cut1 = s.substr(prev, pos);
        // removing spaces
        cut1 = edit(cut1);
        deque->push_back(cut1);
        *sizeDeque += 1;
        // checking if the next char is a sign too
        string cut2 = s.substr(pos, s.length());
        s = cut2;
        string sign;
        if (isSign(cut2[1])) {
            sign = s.substr(0, 2);
            afterSign = s.substr(2, s.length());
        } else {
            sign = s.substr(0, 1);
            afterSign = s.substr(1, s.length());
        }
        deque->push_back(sign);
        *sizeDeque += 1;
    }
    // removing the '{'
    if ((pos = afterSign.find_first_of("{", prev)) != std::string::npos) {
        afterSign = afterSign.substr(0, pos);
    }
    afterSign = edit(afterSign);
    deque->push_back(afterSign);
    deque->push_back("{");
    *sizeDeque += 2;
}

bool isSign(char &c) {
    if (c == '<' || c == '>' || c == '=') {
        return true;
    }
    return false;
}

string edit(string s) {
    size_t prev = 0, pos;
    while ((pos = s.find_first_of(" ", prev)) != std::string::npos) {
        string cut1 = s.substr(prev, pos);
        string cut2 = s.substr(pos + 1, s.length());
        s = cut1 + cut2;
    }
    return s;
}