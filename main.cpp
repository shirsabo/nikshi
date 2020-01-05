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
#include <mutex>
#include "AssignCommand.h"

int sizeAr = 0;
using namespace std;
unordered_map<string, Var *> s_map;
unordered_map<string, Var *> v_map;
std::mutex s_map_mutex;
std::mutex v_map_mutex;

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

void enterLIneVar(string s, deque<string> *deque, int *sizeDeque);

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
    s_map = *(server_map);
    // creating var table
    unordered_map<string, Var *> *varTable = new unordered_map<string, Var *>;
    v_map = *(varTable);
    // creating a map of the commands
    unordered_map<string, Command *> mp;
    createMap(&mp, varTable, server_map, offWhileServer);
    parser(&mp, array, sizeAr, offWhileServer);
    deleteProject(server_map, varTable, &mp);

    return 0;
}

/** deleting the commands and the vars **/
void deleteProject(unordered_map<string, Var *> *serverMap, unordered_map<string, Var *> *varTable,
                   unordered_map<string, Command *> *mp) {
    std::lock_guard<std::mutex> guard(s_map_mutex);
    for (auto &iter:*serverMap) {
        delete iter.second;
    }
    s_map_mutex.unlock();
    std::lock_guard<std::mutex> guard1(v_map_mutex);
    for (auto &iter:*varTable) {
        delete iter.second;
    }
    v_map_mutex.unlock();
    for (auto &iter:*mp) {
        delete iter.second;
    }
}

/** reading the text file and execute accordingly**/
void parser(unordered_map<string, Command *> *mp, string *array, int size, int *offWhileServer) {
    int index = 0;
    thread t3;
    thread t4;
    Command *c, *m;
    OpenServerCommand *c1;
    ConnectCommand *m2;
    // executing the first two lines
    for (int i = 0; i < 2; i++) {
        if ((array[index] != "openDataServer") && array[index] != "connectControlClient") {
            continue;
        }
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

/** going through the array and executing the command accordingly **/
void iterateParser(int size, unordered_map<string, Command *> *mp, int *index, string *array) {
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
        }
    }
}

/** creating a map of commands with the suitable keys according to specific words in the text file **/
void createMap(unordered_map<string, Command *> *pMap, unordered_map<string, Var *> *varTable,
               unordered_map<string, Var *> *server_map, int *offWhileServer) {
    OpenServerCommand *openCommand = new OpenServerCommand(server_map, offWhileServer, &s_map_mutex);
    ConnectCommand *connect = new ConnectCommand(varTable, &v_map_mutex);
    DefineVarCommand *varCommand = new DefineVarCommand(varTable, server_map, &s_map_mutex, &v_map_mutex);
    PrintCommand *print = new PrintCommand(varTable, &v_map_mutex);
    SleepCommand *sleep = new SleepCommand(varTable, &v_map_mutex);
    AssignCommand *assign = new AssignCommand(varTable, connect, &v_map_mutex);
    //must be the last one we enter because it has a map of the commands too
    ConditionParser *ifCommand = new IfCommand(pMap, varTable, &v_map_mutex);
    ConditionParser *loopCommand = new LoopCommand(pMap, varTable, &v_map_mutex);
    // entering all the command and their keys to the map
    pMap->insert(pair<string, Command *>("openDataServer", openCommand));
    pMap->insert(pair<string, Command *>("connectControlClient", connect));
    pMap->insert(pair<string, Command *>("var", varCommand));
    pMap->insert(pair<string, Command *>("Print", print));
    pMap->insert(pair<string, Command *>("Sleep", sleep));
    pMap->insert(pair<string, Command *>("while", loopCommand));
    pMap->insert(pair<string, Command *>("if", ifCommand));
    pMap->insert(pair<string, Command *>("=", assign));
}

/** going through the text file and removing unneccessery chars. at the end entering all the words in the text
    to an array **/
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
                if (sub1 == "var") {
                    // entering the line as needed to the deque
                    enterLIneVar(sub2, &deque, &sizeDeque);
                    prev = 0;
                    break;
                }
                if (line[pos] == '=') {
                    // entering all the line to the deque after removing the spaces
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
                    // entering all the line to the deque after removing the spaces
                    enterLine('=', &deque, pos, &prev, line, &sizeDeque);
                    break;
                } else {
                    string sub2 = line.substr(pos - prev + 1, line.length());
                    line = sub2;
                    prev = 0;
                    continue;
                }
            }
            line = line.substr(pos + 1, line.length());
            prev = 0;
        }
        if ((line[0] == ' ') || (line[0] == ',') || (line[0] == '(') || (line[0] == ')') || (line[0] == '\t')) {
            line = line.substr(1, line.length());
        }
        // printing the {,} and the end of the line if it does'nt contain any of the checked chars
        if (((line != "" && line != ")") && (line.find_first_of(" ,(\t)=", prev)) == std::string::npos)
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
    fclose(file);
    return array;
}

/** in case we have a var - entering all the line to the deque after separating the string as needed**/
void enterLIneVar(string s, deque <string> *deque, int *sizeDeque) {
    s = edit(s);
    size_t prev = 0;
    size_t pos = s.find_first_of(" -=<", prev);
    string name = s.substr(prev, pos);
    string direction, sim;
    if (s[pos] == '=') {
        direction = s.substr(pos, 1);
        pos = pos + 1;
        sim = s.substr(pos, s.length());
    } else {
        // in case of <- / ->
        direction = s.substr(pos, 2);
        pos = pos + 2;
        sim = s.substr(pos + 4, s.length() - pos - 5);
    }
    // entering to the deque
    deque->push_back(name);
    deque->push_back(direction);
    deque->push_back(sim);
    *sizeDeque = *sizeDeque + 3;
}

/** in case we have an equals sign or parenthesis - entering all the line to the deque after removing the spaces **/
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

/** entering the deque to an array as required **/
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

/** entering the line to the deque as needed for the while or the if command **/
void editConditionParser(string s, deque <string> *deque, int *sizeDeque) {
    size_t prev = 0, pos;
    string afterSign = s;
    // looking for a sign so we will know what condition to check
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

/** checking if the char is a specific char that is a sign **/
bool isSign(char &c) {
    if (c == '<' || c == '>' || c == '=') {
        return true;
    }
    return false;
}

/** removing spaces from a line **/
string edit(string s) {
    size_t prev = 0, pos;
    while ((pos = s.find_first_of(" ", prev)) != std::string::npos) {
        string cut1 = s.substr(prev, pos);
        string cut2 = s.substr(pos + 1, s.length());
        s = cut1 + cut2;
    }
    return s;
}