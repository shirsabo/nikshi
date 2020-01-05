# Assignment 3

in this project we were asked to connect the server of "Flightgear" and read values from it and also to connect as a Client and actually send the server updates about variables in the game's algorithm, all that according to "fly.txt".
### How to compile?
```sh
g++ -std=c++14 *.cpp -Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic -o a.out -pthread
```
### How to run?
```sh
./a.out fly.txt
```
- this project has a serious amount of code and because of that this file will make everything clear to you.
### The main
>The lexer
```sh
string *lexer(char *argv[]);
```
in this function we creates a "Mega" string that will contain the text file in one String seperated with ",", and returns String * .
```sh
word1,word2,......,wordK
```
Functions that we creat in order to edit the text file:
```sh
 void enterLine(char token, deque <string> *deque, size_t pos, size_t *prev, string line, int *sizeDeque):
```
 in case we have an equals sign or parenthesis - entering all the line to the deque after removing the spaces
 ```sh
void editConditionParser(string s, deque <string> *deque, int *sizeDeque) :
```
entering the line to the deque as needed for the while or the if command.
 ```sh
 string edit(string s)
 ```
entering the line to the deque as needed for the while or the if command .
>Creating the maps
```sh
   createMap(&mp, varTable, server_map, offWhileServer);
```
in this function we make all the Commands know the adresses of the maps:
```sh
 unordered_map<string, Var *> *varTable
```
```sh
unordered_map<string, Var *> *server_map
```
also, we take care of the map of the Comands:
```sh
unordered_map<string, Command *> *pMap
```
we insert to the map  <key,command> of any possible kind: "if"," Print","Sleep",etc.
```sh
For example:
    pMap->insert(pair<string, Command *>("openDataServer", openCommand));
```
Note: in this program only single Command* is Created for each type of Command.
>The parser
```sh
  void parser(unordered_map<string, Command *> *mp, string *array, int size, int *offWhileServer)
```

one of the main functions of this program, in here all the threads created:

| Thread | Function |
| ------ | ------ |
| thread t1(&OpenServerCommand::acceptence, ref((c1)), &(array[index + 1])) | waiting until acceptence from server |
|  thread t2(&OpenServerCommand::initializeServerMap, ref((c1))) |  initialize server map|
|thread t3(&OpenServerCommand::dataEntryPoint, ref((c1))) | while loop that is "alive" after acceptence until exit 0, in this thread the program gets updates from server|
| thread t4(&ConnectCommand::connection, ref(m2), &array[index + 1]) | make a connection as a client to the simulator|

after creating threads the program calls to
```sh
   iterateParser(size, mp, &index, array);
```
    while (not empty) {
        auto pos = mp->find(array[*index]);
        if (pos == mp->end()) {
        throw "problem with lexer;
        } else {
            Command *c = pos->second;
            *index += c->execute(&array[*index + 1]);

as we can see ' here we are calling to the "Execute" function, and by that all the command are executed.
```sh
  void deleteProject(unordered_map<string, Var *> *serverMap, unordered_map<string, Var *> *varTable,unordered_map<string, Command *> *mp) ;
```
 deleting the commands and the vars
### Var.cpp
 * a var has a name, sim, direction and a value.
 * we use the value of the var in order to know the values of the vars defined in the text file.
 * if the direction in towards outside we will want to update the flight simulator according to the
 * values of the vars we created in our program.
 * if the direction in towards inside we will get the value of the var from the flight simulator.
 * we might create a var and want to assign a value we choose, in this case we will see the equals sign and use the shunting yard in order to calculate it.
 
```sh
Var::Var(string nameIn, string simIn, string inoutIn)
```
### SleepCommand.cpp
```sh
SleepCommand::SleepCommand(unordered_map<string, Var *> *pMap)
```
Note: the sleep command knows the var table in case of usig shunting yard.
```sh
int SleepCommand::execute(string *s)
```
 calculating the number of seconds we want to sleep by the shunting yard.
 ### ShuntingYard.cpp
  the shunting yard class takes a string that has variables, numbers and mathmatical signs and
 * creates an expression out of it.
 * in order to calculates the expression we use the map provided in the setVariables func.
 * we pay attention to order and the precedence.
### PrintCommand.cpp
 ```sh
PrintCommand::PrintCommand(unordered_map <string, Var*> * varTableIn)
```
Note that  it kmows the varTable.
 ```sh
 int PrintCommand::execute(std::__cxx11::string *print)
 ```
  printing a sentence or a number we calculate using the shunting yard.
### IfCommand.cpp
 ```sh
IfCommand::IfCommand(unordered_map<string, Command *> *mapCommandIn, unordered_map<string, Var *> *varTableIn)
 ```
  ```sh
int IfCommand::execute(string *s)
 ```
 uses the executeHelper func of ConditionParser, returning from it the number of steps we need to jump in the array until the end of the parenthesis.

### LoopCommand.cpp
  ```sh
LoopCommand::LoopCommand(unordered_map<string, Command *> *mapCommandIn, unordered_map<string, Var *> *varTableIn)
 ```
   ```sh
   int LoopCommand::execute(string *s)
 ```
 uses the executeHelper func of ConditionParser.
 * if the condition in the loop is false - returning from it the number of steps we need to jump in the array until the end of the parenthesis.
 * if the condition is true - returning 0 so we will execute it again.
 ### DefineVarCommand.cpp
   ```sh
DefineVarCommand::DefineVarCommand(unordered_map<string, Var *> *varTableIn, unordered_map<string, Var *> *server_map,  mutex *muteServerMap, mutex *varMute)
 ```
  ```sh
int DefineVarCommand::execute(string *s);
 ```
  creating a new var according to the text file.
 * if the sign is -> we will create a new one,
 * if the sign is <- we will look for it in the map of the vares that were created in the server command
 * if the sign is = we will create it and assign a value to it.

Install the dependencies and devDependencies and start the server.

```sh
 bool DefineVarCommand::checkForErrow(string *s) ;
 ```
 returns true if the string has ->/<- and false otherwise.
 ## ConditionParser.cpp
 ```sh
 void ConditionParser::parser(unordered_map<string, Command *> *mp, string *array, int size)
  ```
  going through the commands between the parenthesis and executing them
For production environments.
 ```sh
 bool ConditionParser::checkCondition(string *original)
   ```
checking the condition of the Command so we will know if to execute the commands inside the parenthesis.
```sh
int ConditionParser::executeHelper(string *s)
```
counting how many places in the array belong to this command so we'll know what number to return
   from the execute func. executing the commands inside the parenthesis accordingly.
### AssignCommand.cpp
```sh
AssignCommand::AssignCommand(unordered_map<string, Var *> *varTableIn, ConnectCommand *connectIn)
```
```sh
int AssignCommand::execute(string * s)
```
 changing a value of a var by calculating the value in the shunting yard.
  ### OpenServerCommand.cpp
  ```sh
  OpenServerCommand::OpenServerCommand(std::unordered_map<string, Var *> *pMap, int *offWhileServerIn)
 ```
   ```sh
 int OpenServerCommand::dataEntryPoint() ;
  ```
   receiving the information from the server and updating the var's values until we finish reading the text file.
 ```sh
   void OpenServerCommand::initializeServerMap();
```
receiving the information from the server and updating the var's values.
 ```sh
void OpenServerCommand::updateMap(string buffer, bool firstTime);
```
using the buffer to update the var's values by separating the numbers between the commands.
 ```sh
void OpenServerCommand::acceptence(string *s) ;
```
 ```sh
string OpenServerCommand::initializeVars(string sub, int i, bool firstTime);
```
creating the vars and updating their sim and name by the generic_small file.
 * updating their direction to be towards outside and their values to be 0.
 * the string we send back is the sim of the var we entered it's i
  ```sh
void OpenServerCommand::notFirstRead(string sub, int i)
```
updating the var's values according to the buffer we got from the server.
```sh
string OpenServerCommand::readOneChar()
```
 reading the information from the server char by char until finding /n, combibng all the chars and returning the string so we can use it to update the var's values.

### ConnectCommand.cpp
```sh
ConnectCommand::ConnectCommand(unordered_map<string, Var *> *pMap)
```
```sh
int ConnectCommand::execute(string *s);
```
executing the command - creating a client.
```sh
void ConnectCommand::connection(string *s);
```
 connecting to the client, printing a message if could not connect to host server and exiting.
 ```sh
 void ConnectCommand::changeValue(string sim, double value) ;
 ```
 changing a var's value in the simulator according to the text file.
  ```sh
void ConnectCommand::clientSetter(int socket);
 ```
  initializing the client socket fiels in this class.
   ```sh
  string ConnectCommand::editSim(string *sim);
  ```
  removing the " sign from the sim.

```sh
  ConnectCommand:: ~ConnectCommand()
   ```
closing the client socket.
