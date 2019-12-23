//
// Created by nikol on 11/12/2019.
//
#include "OpenServerCommand.h"
#include <sys/socket.h>
#include <iostream>
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>
#include <deque>

using namespace std;

int OpenServerCommand::execute(string *s) {

    return 2;
}

OpenServerCommand::OpenServerCommand(std::unordered_map<string, Var *> *pMap, int *offWhileServerIn) {
    this->varTable = pMap;
    this->offWhileServer = offWhileServerIn;
}

int OpenServerCommand::dataEntryPoint(string *s) {
    //reading from client
    while (!*offWhileServer) {
        string buffer = readOneChar();
        updateMap(buffer, false);
    }
    return 0;
}

void OpenServerCommand::initializeServerMap(string *s) {
    //reading one char at a time
    string buffer = readOneChar();
    cout << "first time" << endl;
    updateMap(buffer, true);
}

void OpenServerCommand::updateMap(string buffer, bool firstTime) {
    //buffer = "1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,32,33,34,35,36";
    int i = 1;
    string s = buffer, sub;
    cout << s << endl;
    size_t prev = 0, pos;
    for (; (pos = s.find_first_of(",", prev)) != std::string::npos; i += 1) {
        if (pos > prev) {
            sub = s.substr(prev, pos - prev);
            if (firstTime) {
                // creating the server map by creating new vars
                string s = initializeVars(sub, i, true);
            } else {
                // updating the server map by the new values
                notFirstRead(sub, i);
            }
            string sub2 = s.substr(pos - prev + 1, s.length());
            s = sub2;
            prev = 0;
            continue;
        }
        prev = pos + 1;
    }
    if (firstTime) {
        // i = last
        initializeVars(s, 36, true);
    } else {
        notFirstRead(s, 36);
    }
}

int OpenServerCommand::acceptence(string *s) {
    int socketfd = socket(AF_INET, SOCK_STREAM, 0);
    if (socketfd == -1) {
        //error
        throw "Bad connedction";
    }
    //bind socket to IP address
    // we first need to create the sockaddr obj.
    sockaddr_in address; //in means IP4
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY; //give me any IP allocated for my machine
    address.sin_port = htons(stoi(*s));
    //we need to convert our number to a number that the network understands.
    //the actual bind command
    if (bind(socketfd, (struct sockaddr *) &address, sizeof(address)) == -1) {
        cout<< "Bad connedction\n";
    }
    //making socket listen to the port
    if (listen(socketfd, 5) == -1) { //can also set to SOMAXCON (max connections)
        throw "Bad connedction\n";
    } else {
        std::cout << "Server is now listening ..." << std::endl;
    }
//waiting until connection
    // accepting a client
    int client_socket = accept(socketfd, (struct sockaddr *) &address,
                               (socklen_t *) &address);
    if (client_socket == -1) {
        throw "Bad connedction\n";
    }
    clientSetter(client_socket);
    close(socketfd); //closing the listening socket
}

void OpenServerCommand::clientSetter(int socket) {
    this->client_socket = socket;
}

string OpenServerCommand::initializeVars(string sub, int i, bool firstTime) {
    Var *varTemp;
    switch (i) {
        case 1://
            if (firstTime) {
                cout << "first assign openServerCommand" << endl;
                varTemp = new Var("airspeed-indicator_indicated-speed-kt",
                                  "/instrumentation/airspeed-indicator/indicated-speed-kt", "<-");
                varTemp->setValue(stof(sub));
                this->varTable->insert({"\"/instrumentation/airspeed-indicator/indicated-speed-kt\"", varTemp});
            }
            return "\"/instrumentation/airspeed-indicator/indicated-speed-kt\"";
        case 5:
            //
            if (firstTime) {
                varTemp = new Var("altimeter_indicated-altitude-ft",
                                  "/instrumentation/altimeter/indicated-altitude-ft", "<-");
                varTemp->setValue(stof(sub));
                this->varTable->insert({"\"/instrumentation/altimeter/indicated-altitude-ft\"", varTemp});
            }
            return "\"/instrumentation/altimeter/indicated-altitude-ft\"";
        case 6:
            //
            if (firstTime) {
                varTemp = new Var("altimeter_pressure-alt-ft",
                                  "/instrumentation/altimeter/pressure-alt-ft", "<-");
                varTemp->setValue(stof(sub));
                this->varTable->insert({"\"//instrumentation/altimeter/pressure-alt-ft\"", varTemp});
            }
            return "\"//instrumentation/altimeter/pressure-alt-ft\"";
        case 8:
            if (firstTime) {
                varTemp = new Var("attitude-indicator_indicated-roll-deg",
                                  "/instrumentation/attitude-indicator/indicated-roll-deg", "<-");
                varTemp->setValue(stof(sub));
                this->varTable->insert({"\"/instrumentation/attitude-indicator/indicated-roll-deg\"", varTemp});
            }
            return "\"/instrumentation/attitude-indicator/indicated-roll-deg\"";
        case 12:
            if (firstTime) {
                varTemp = new Var("encoder_pressure-alt-ft",
                                  "/instrumentation/encoder/pressure-alt-ft", "<-");
                varTemp->setValue(stof(sub));
                this->varTable->insert({"\"/instrumentation/encoder/pressure-alt-ft\"", varTemp});
            }
            return "\"/instrumentation/encoder/pressure-alt-ft\"";
        case 10:
            if (firstTime) {
                varTemp = new Var("attitude-indicator_internal-roll-deg",
                                  "/instrumentation/attitude-indicator/internal-roll-deg", "<-");
                varTemp->setValue(stof(sub));
                this->varTable->insert({"\"/instrumentation/attitude-indicator/internal-roll-deg\"", varTemp});
            }
            return "\"/instrumentation/attitude-indicator/internal-roll-deg\"";
        case 2://
            if (firstTime) {
                varTemp = new Var("time_warp",
                                  "/sim/time/warp", "<-");
                varTemp->setValue(stof(sub));
                this->varTable->insert({"\"/sim/time/warp\"", varTemp});
            }
            return "\"/sim/time/warp\"";
        case 3://
            if (firstTime) {
                varTemp = new Var("switches_magnetos",
                                  "/controls/switches/magnetos", "<-");
                varTemp->setValue(stof(sub));
                this->varTable->insert({"\"/controls/switches/magnetos\"", varTemp});
            }
            return "\"/controls/switches/magnetos\"";
        case 4:
            if (firstTime) {
                varTemp = new Var("heading-indicator_offset-deg",
                                  "/instrumentation/heading-indicator/offset-deg", "<-");
                varTemp->setValue(stof(sub));
                this->varTable->insert({"\"/instrumentation/heading-indicator/offset-deg\"", varTemp});
            }
            return "\"/instrumentation/heading-indicator/offset-deg\"";
        case 11:
            if (firstTime) {
                varTemp = new Var("encoder_indicated-altitude-ft",
                                  "/instrumentation/encoder/indicated-altitude-ft", "<-");
                varTemp->setValue(stof(sub));
                this->varTable->insert({"\"/instrumentation/encoder/indicated-altitude-ft\"", varTemp});
            }
            return "\"/instrumentation/encoder/indicated-altitude-ft\"";
        case 13:
            if (firstTime) {
                varTemp = new Var("gps_indicated-altitude-ft",
                                  "/instrumentation/gps/indicated-altitude-ft", "<-");
                varTemp->setValue(stof(sub));
                this->varTable->insert({"\"/instrumentation/gps/indicated-altitude-ft\"", varTemp});
            }
            return "\"/instrumentation/gps/indicated-altitude-ft\"";
        case 14:
            if (firstTime) {
                varTemp = new Var("gps_indicated-ground-speed-kt",
                                  "/instrumentation/gps/indicated-ground-speed-kt", "<-");
                varTemp->setValue(stof(sub));
                this->varTable->insert({"\"/instrumentation/gps/indicated-ground-speed-kt\"", varTemp});
            }
            return "\"/instrumentation/gps/indicated-ground-speed-kt\"";
        case 15:
            if (firstTime) {
                varTemp = new Var("gps_indicated-vertical-speed",
                                  "/instrumentation/gps/indicated-vertical-speed", "<-");
                varTemp->setValue(stof(sub));
                this->varTable->insert({"\"/instrumentation/gps/indicated-vertical-speed\"", varTemp});
            }
            return "\"/instrumentation/gps/indicated-vertical-speed\"";
        case 16:
            if (firstTime) {
                varTemp = new Var("indicated-heading-deg",
                                  "/instrumentation/heading-indicator/indicated-heading-deg", "<-");
                varTemp->setValue(stof(sub));
                this->varTable->insert({"\"/instrumentation/heading-indicator/indicated-heading-deg\"", varTemp});
            }
            return "\"/instrumentation/heading-indicator/indicated-heading-deg\"";
        case 17:
            if (firstTime) {
                varTemp = new Var("magnetic-compass_indicated-heading-deg",
                                  "/instrumentation/magnetic-compass/indicated-heading-deg", "<-");
                varTemp->setValue(stof(sub));
                this->varTable->insert({"\"/instrumentation/magnetic-compass/indicated-heading-deg\"", varTemp});
            }
            return "\"/instrumentation/magnetic-compass/indicated-heading-deg\"";
        case 18:
            if (firstTime) {
                varTemp = new Var("slip-skid-ball_indicated-slip-skid",
                                  "/instrumentation/slip-skid-ball/indicated-slip-skid", "<-");
                varTemp->setValue(stof(sub));
                this->varTable->insert({"\"/instrumentation/slip-skid-ball/indicated-slip-skid\"", varTemp});
            }
            return "\"/instrumentation/slip-skid-ball/indicated-slip-skid\"";
        case 19:
            if (firstTime) {
                varTemp = new Var("turn-indicator_indicated-turn-rate",
                                  "/instrumentation/turn-indicator/indicated-turn-rate", "<-");
                varTemp->setValue(stof(sub));
                this->varTable->insert({"\"/instrumentation/turn-indicator/indicated-turn-rate\"", varTemp});
            }
            return "\"/instrumentation/turn-indicator/indicated-turn-rate\"";
        case 20:
            if (firstTime) {
                varTemp = new Var("vertical-speed-indicator_indicated-speed-fpm",
                                  "/instrumentation/vertical-speed-indicator/indicated-speed-fpm", "<-");
                varTemp->setValue(stof(sub));
                this->varTable->insert({"\"/instrumentation/vertical-speed-indicator/indicated-speed-fpm\"", varTemp});
            }
            return "\"/instrumentation/vertical-speed-indicator/indicated-speed-fpm\"";
        case 21:
            if (firstTime) {
                varTemp = new Var("flight_aileron",
                                  "/controls/flight/aileron", "<-");
                varTemp->setValue(stof(sub));
                this->varTable->insert({"\"/controls/flight/aileron\"", varTemp});
            }
            return "\"/controls/flight/aileron\"";
        case 22:
            if (firstTime) {
                varTemp = new Var("flight_elevator",
                                  "/controls/flight/elevator", "<-");
                varTemp->setValue(stof(sub));
                this->varTable->insert({"\"/controls/flight/elevator\"", varTemp});
            }
            return "\"/controls/flight/elevator\"";
        case 23:
            if (firstTime) {
                varTemp = new Var("flight_rudder",
                                  "/controls/flight/rudder", "<-");
                varTemp->setValue(stof(sub));
                this->varTable->insert({"\"/controls/flight/rudder\"", varTemp});
            }
            return "\"/controls/flight/rudder\"";
        case 24:
            if (firstTime) {
                varTemp = new Var("flight_flaps",
                                  "/controls/flight/flaps", "<-");
                varTemp->setValue(stof(sub));
                this->varTable->insert({"\"/controls/flight/flaps\"", varTemp});
            }
            return "\"/controls/flight/flaps\"";
        case 25:
            if (firstTime) {
                varTemp = new Var("engine_throttle",
                                  "/controls/engines/engine/throttle", "<-");
                varTemp->setValue(stof(sub));
                this->varTable->insert({"\"/controls/engines/engine/throttle\"", varTemp});
            }
            return "\"/controls/engines/engine/throttle\"";
        case 26:
            if (firstTime) {
                varTemp = new Var("current-engine_throttle",
                                  "/controls/engines/current-engine/throttle", "<-");
                varTemp->setValue(stof(sub));
                this->varTable->insert({"\"/controls/engines/current-engine/throttle\"", varTemp});
            }
            return "\"/controls/engines/current-engine/throttle\"";
        case 27:
            if (firstTime) {
                varTemp = new Var("switches_master-avionicse",
                                  "/controls/switches/master-avionics", "<-");
                varTemp->setValue(stof(sub));
                this->varTable->insert({"\"/controls/switches/master-avionics\"", varTemp});
            }
            return "\"/controls/switches/master-avionics\"";
        case 28:
            if (firstTime) {
                varTemp = new Var("switches_starter",
                                  "/controls/switches/starter", "<-");
                varTemp->setValue(stof(sub));
                this->varTable->insert({"\"/controls/switches/starter\"", varTemp});
            }
            return "\"/controls/switches/starter\"";
        case 29:
            if (firstTime) {
                varTemp = new Var("active-engine_auto-start",
                                  "/engines/active-engine/auto-start", "<-");
                varTemp->setValue(stof(sub));
                this->varTable->insert({"\"/engines/active-engine/auto-start\"", varTemp});
            }
            return "\"/engines/active-engine/auto-start\"";
        case 30:
            if (firstTime) {
                varTemp = new Var("flight_speedbrake",
                                  "/controls/flight/speedbrake", "<-");
                varTemp->setValue(stof(sub));
                this->varTable->insert({"\"/controls/flight/speedbrake\"", varTemp});
            }
            return "\"/controls/flight/speedbrake\"";
        case 31:
            if (firstTime) {
                varTemp = new Var("c172p_brake-parking",
                                  "/sim/model/c172p/brake-parking", "<-");
                varTemp->setValue(stof(sub));
                this->varTable->insert({"\"/sim/model/c172p/brake-parking\"", varTemp});
            }
            return "\"/sim/model/c172p/brake-parking\"";
        case 32:
            if (firstTime) {
                varTemp = new Var("engine_primer",
                                  "/controls/engines/engine/primer", "<-");
                varTemp->setValue(stof(sub));
                this->varTable->insert({"\"/controls/engines/engine/primer\"", varTemp});
            }
            return "\"/controls/engines/engine/primer\"";
        case 33:
            if (firstTime) {
                varTemp = new Var("current-engine_mixture",
                                  "/controls/engines/current-engine/mixture", "<-");
                varTemp->setValue(stof(sub));
                this->varTable->insert({"\"/controls/engines/current-engine/mixture\"", varTemp});
            }
            return "\"/controls/engines/current-engine/mixture\"";
        case 34:
            if (firstTime) {
                varTemp = new Var("switches_master-bat",
                                  "/controls/switches/master-bat", "<-");
                varTemp->setValue(stof(sub));
                this->varTable->insert({"\"/controls/switches/master-bat\"", varTemp});
            }
            return "\"/controls/switches/master-bat\"";
        case 35:
            if (firstTime) {
                varTemp = new Var("switches_master-alt",
                                  "/controls/switches/master-alt", "<-");
                varTemp->setValue(stof(sub));
                this->varTable->insert({"\"/controls/switches/master-alt\"", varTemp});
            }
            return "\"/controls/switches/master-alt\"";
        case 36:
            if (firstTime) {
                varTemp = new Var("engine_rpm",
                                  "/engines/engine/rpm", "<-");
                varTemp->setValue(stof(sub));
                this->varTable->insert({"\"/engines/engine/rpm\"", varTemp});
            }
            return "\"/engines/engine/rpm\"";
        case 7:
            if (firstTime) {
                varTemp = new Var("attitude-indicator_indicated-pitch-deg",
                                  "/instrumentation/attitude-indicator/indicated-pitch-deg", "<-");
                varTemp->setValue(stof(sub));
                this->varTable->insert({"\"/instrumentation/attitude-indicator/indicated-pitch-deg\"", varTemp});
            }
            return "\"/instrumentation/attitude-indicator/indicated-pitch-deg\"";
        case 9:
            if (firstTime) {
                cout << "last assign openServerCommand" << endl;
                varTemp = new Var("attitude-indicator_internal-pitch-deg",
                                  "/instrumentation/attitude-indicator/internal-pitch-deg", "<-");
                varTemp->setValue(stof(sub));
                this->varTable->insert({"\"/instrumentation/attitude-indicator/internal-pitch-deg\"", varTemp});
            }
            return "\"/instrumentation/attitude-indicator/internal-pitch-deg\"";
        default:
            return "";
    }
}

void OpenServerCommand::notFirstRead(string sub, int i) {
    string sim;
    if (i == 36) {
        cout<<"rpm-------------------------------------------------------------"<<endl;
    }
    sim = initializeVars(sub, i, false);
    // finding the var from the server map
    if (this->varTable == NULL) {
        std::cout << "erorrrrrrrrrrrrrrrrrrrrrrrrrrrrrr" << endl;
    }
    auto pos = this->varTable->find(sim);
    if (pos == this->varTable->end()) {
        // check if it's assignment line (rpm = 0)
        /*** error ***/
        cout << "error - problem in open server command: not first read" << endl;
    } else {
        pos->second->setValue(stof(sub));
        //cout << "changing the server map: "+ pos->first+ " " + sub<< endl;
    }
}

string OpenServerCommand::readOneChar() {
    string line = "";
    char buffer[1] = {0};
    int flag = 1;
    while (flag) {
        int valread = ::read(client_socket, buffer, 1);
        if (buffer[0] != '\n') {
            line = line + buffer;
        } else {
            flag = 0;
        }
    }
    return line;
}

