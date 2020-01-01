//
// Created by oem on 18/12/2019.
//

#include "ShuntingYard.h"
#include "Var.h"
#include <algorithm>
#include <stdlib.h>
#include <iostream>
#include <map>
#include <unordered_map>

/** the shunting yard class takes a string that has variables, numbers and mathmatical signs and
 * creates an expression out of it.
 * in order to calculates the expression we use the map provided in the setVariables func.
 * we pay attention to order and the precedence.**/

BinaryOperator::BinaryOperator(Expression *left, Expression *right) : left1(left), right1(right) {}

BinaryOperator::~BinaryOperator() {
}

Expression *BinaryOperator::getRight() const {
    return right1;
}

Expression *BinaryOperator::getLeft() const {
    return left1;
}


UnaryOperator::UnaryOperator(Expression *expression) : expression1(expression) {}

UnaryOperator::~UnaryOperator() {
}

double UnaryOperator::calculate() {
    return expression1->calculate();
}

void UnaryOperator::setExpression(Expression *expression) {
    UnaryOperator::expression1 = expression;
}

Expression *UnaryOperator::getExpression() const {
    return expression1;
}


Value::Value(double val) : val1(val) {}

double Value::calculate() {
    return val1;
}

Value::~Value() {}


Variable::Variable(const string &name, double value) : name1(name), value1(value) {}

double Variable::calculate() {
    return value1;
}

void Variable::setVariable(double val) {
    value1 = val;
}

double Variable::getVal() const {
    return value1;
}

const string &Variable::getName() const {
    return name1;
}

Variable &Variable::operator++(int) {
    this->setVariable(this->getVal() + 1);
    return *this;
}

Variable &Variable::operator--() {
    this->setVariable(this->getVal() - 1);
    return *this;
}

Variable &Variable::operator++() {
    this->setVariable(this->getVal() + 1);
    return *this;
}

Variable &Variable::operator--(int) {
    this->setVariable(this->getVal() - 1);
    return *this;
}

Variable &Variable::operator+=(double num) {
    this->setVariable(value1 + num);
    return *this;
}

Variable &Variable::operator-=(double num) {
    this->setVariable(value1 - num);
    return *this;
}

Variable::~Variable() {}


UPlus::UPlus(Expression *expression) : UnaryOperator(expression) {}

double UPlus::calculate() {
    return this->getExpression()->calculate();
}

UPlus::~UPlus() {
    delete this->getExpression();
}


UMinus::UMinus(Expression *expression) : UnaryOperator(expression) {}

double UMinus::calculate() {
    return -1 * this->getExpression()->calculate();
}

UMinus::~UMinus() {
    delete this->getExpression();
}


Plus::Plus(Expression *left, Expression *right) : BinaryOperator(left, right) {}

double Plus::calculate() {
    return getRight()->calculate() + getLeft()->calculate();
}

Plus::~Plus() {
    delete this->getLeft();
    delete this->getRight();
}


Minus::Minus(Expression *left, Expression *right) : BinaryOperator(left, right) {
}

double Minus::calculate() {
    return getLeft()->calculate() - getRight()->calculate();
}

Minus::~Minus() {
    delete this->getLeft();
    delete this->getRight();
}


Mul::Mul(Expression *left, Expression *right) : BinaryOperator(left, right) {}

double Mul::calculate() {
    return getLeft()->calculate() * getRight()->calculate();
}

Mul::~Mul() {
    delete this->getLeft();
    delete this->getRight();
}


Div::Div(Expression *left, Expression *right) : BinaryOperator(left, right) {}

double Div::calculate() {
    if (getRight()->calculate() == 0) {
        throw "error - devide in 0";
    }
    return getLeft()->calculate() / getRight()->calculate();
}

Div::~Div() {
    delete this->getLeft();
    delete this->getRight();
}

void Interpreter::setVariables(unordered_map<string, Var *> *varTable) {
// Get an iterator pointing to begining of map
    for (auto it = varTable->begin(); it != varTable->end(); it++) {
        map1.insert({it->first, it->second->getValue()});
    }
}

void Interpreter::setVariables(string s) {
    int n = std::count(s.begin(), s.end(), ';');
    int j = 0;
    int i = 0;
    for (int l = 0; i < n + 1; i++, j = 0) {
        string cut1 = ";";
        l = s.find(cut1);
        string sub = s.substr(j, l);
        // setting the new location to the next search
        j = l + 1;
        string cut2 = "=";
        string sub1 = sub.substr(0, s.find(cut2));
        string sub2 = sub.substr(s.find(cut2) + 1, sizeof(s));
        if (!isNumber(sub2)) {
            throw "unvalid value for a variable";
        } else if (!isNameValid(sub1)) {
            throw "unvalid name for variable";
        }
        // checking if the variable exist
        map<string, double>::iterator it;
        it = this->map1.find(sub1);
        if (it != this->map1.end()) {
            //found this name
            it->second = atof(sub2.c_str());
        } else {
            // didnt find the name
            map1.insert({sub1, atof(sub2.c_str())});
        }
        s = s.substr(j, s.size());
    }
}

bool Interpreter::isNumber(string s) {
    for (unsigned int i = 0; i < s.length(); i++) {
        if ((i == 0) && (isdigit(s[i]))) {
            continue;
        } else if ((i == 0) && (s[i] == '-')) {
            if ((isdigit(s[1]))) {
                continue;
            } else {
                return false;
            }
        } else if (i == 0) {
            return false;
        }
        if (isdigit(s[i]) == 0 && s[i] != '.' && s[i] != '-') {
            return false;
        }
    }
    return true;
}

Expression *Interpreter::interpret(string s) {
    string g1 = "could'nt calculate";
    string g2 = "unvalid input";
    string g3 = "variable wasn't initialized";
    string g4 = "could'nt find '('";
    string g5 = "could'nt find ')'";
    try {
        this->readAndAllocate(s);
        Expression *exp = solve();
        expressionStuck.pop();
        return exp;
    } catch (exception e) {
        if (g1.compare(e.what()) == 0) {
            throw g1;
        } else if (g2.compare(e.what()) == 0) {
            throw g2;
        } else if (g3.compare(e.what()) == 0) {
            throw g3;
        } else if (g4.compare(e.what()) == 0) {
            throw g4;
        } else if (g5.compare(e.what()) == 0) {
            throw g5;
        } else {
            throw "could'nt build an expression";
        }
    }
}

// enters the tokens to the stack and queue
void Interpreter::readAndAllocate(string s) {
    char previous = 0;
    int flag = 0;
    // reading every char and entering them to the right stack
    for (unsigned int i = 0; i < s.length(); i++) {
        flag = 1;
        char sub = s[i];
        if (this->checkIfOperator(sub)) {
            if ((outputOueue.size() != 0) && (outputOueue.back() != ',')) {
                this->outputOueue.push(',');
            }
            if ((sub == ')') && (i < s.length() - 1) && (this->checkIfOperator(s[i + 1]))) {
                // checking validation : exp (x)(x)
                if (s[i + 1] == '(') {
                    throw "unvalid input";
                }
            } else if (sub == '(' && isdigit(previous)) {
                // 3(x)
                throw "unvalid input";
            } else if ((sub != '(' && sub != ')') && (previous != '(' && previous != ')') &&
                       checkIfOperator(previous)) {
                // 3+-+-4
                throw "unvalid input";
            }
            // the token is an operator
            this->manageOperators(sub, previous);
        } else {
            if (previous == ')') {
                //(x)3
                throw "unvalid input";
            }
            // the token is a number or "."
            this->outputOueue.push(sub);
        }
        previous = sub;
    }
    if (flag == 1) {
        moveOperators();
    } else {
        throw "unvalid input";
    }
}

int Interpreter::checkIfOperator(char sub) {
    if ((sub == '+') || (sub == '-') || (sub == '*') || (sub == '/') || (sub == '(')
        || (sub == ')') || (sub == '#') || (sub == '$')) {
        return 1;
    } else {
        return 0;
    }
}

void Interpreter::manageOperators(char enterOp, char previous) {
    //checking the precedence of the entered operator and the last one in the stack
    if (this->operatorStuck.empty() && enterOp != ')') {
        this->operatorStuck.push(enterOp);
    } else if (this->operatorStuck.empty() && enterOp == ')') {
        throw "unvalid input";
    } else {
        char stuckOp = this->operatorStuck.top();
        if (precedence(enterOp, stuckOp, previous) == 0) {
            // the entered operator has higher precedence
            this->operatorStuck.push(enterOp);
        } else if (precedence(enterOp, stuckOp, previous) == 1) {
            // the entered operator has less/equal precedence
            this->outputOueue.push(this->operatorStuck.top());
            this->operatorStuck.pop();
            while (precedence(enterOp, stuckOp, previous) == 0) {
                this->outputOueue.push(this->operatorStuck.top());
                this->operatorStuck.pop();
            }
            // entering the last given operator
            this->operatorStuck.push(enterOp);
        } else if (precedence(enterOp, stuckOp, previous) == 3) {
            // UPlus
            this->operatorStuck.push('#');
        } else if (precedence(enterOp, stuckOp, previous) == 4) {
            //UMinus
            this->operatorStuck.push('$');
        } else {
            // the token is ')'
            while ((operatorStuck.size() != 0) && this->operatorStuck.top() != '(') {
                this->outputOueue.push(this->operatorStuck.top());
                this->operatorStuck.pop();
            }
            if (operatorStuck.empty() || operatorStuck.top() != '(') {
                throw "could'nt find '('";
            }
            // removing the '('
            this->operatorStuck.pop();
        }
    }
}

// returns 1 if the stuck operator has bigger oe equal precedence and 0 if not, and 2 if its ')'
// return 3 for UPlus, 4 FOR UMinus
int Interpreter::precedence(char enter, char stuck, char previous) {
    switch (enter) {
        case '+':
            if ((previous == 0) || (previous == '(')) {
                return 3;
            }
            if (stuck == '(') {
                return 0;
            }
            return 1;
        case '-':
            if ((previous == 0) || (previous == '(')) {
                return 4;
            }
            if (stuck == '(') {
                return 0;
            }
            return 1;
        case '*':
            if ((stuck == '+') || (stuck == '-') || (stuck == '(')) {
                return 0;
            } else if ((stuck == '*') || (stuck == '/') || (this->checkIfOperator(previous) == 1)) {
                return 1;
            }
            break;
        case '/':
            if ((stuck == '+') || (stuck == '-') || (stuck == '(')) {
                return 0;
            } else if ((stuck == '*') || (stuck == '/')) {
                return 1;
            }
            break;
        case '(':
            return 0;
        case ')':
            return 2;
        default:
            break;
    }
    // just for the compiler
    return 0;
}

void Interpreter::moveOperators() {
    while (!this->operatorStuck.empty()) {
        if (this->operatorStuck.size() != 0 && this->operatorStuck.top() != '(') {
            this->outputOueue.push(this->operatorStuck.top());
            this->operatorStuck.pop();
        } else if (this->operatorStuck.top() == '(') {
            throw "could'nt find ')'";
        }
    }
}

Expression *Interpreter::solve() {
    // building an expression
    while (this->outputOueue.size() != 0) {
        char token = this->outputOueue.front();
        if (this->checkIfOperator(token)) {
            // the token is an operator
            makeExpressionOperator(token);
        } else {
            if (isalpha(token)) {
                // a letter - create a variable
                makeExpressionVariable(token);
            } else if (isdigit(token)) {
                // a number - create a value
                makeExpressionVar(token);
            }
            if (this->outputOueue.front() == ',') {
                this->outputOueue.pop();
            }
        }
    }
    return expressionStuck.top();
}

void Interpreter::makeExpressionOperator(char token) {
    // the token is an operator
    Expression *e1;
    Expression *e2;
    switch (token) {
        case '+':
            outputOueue.pop();
            if (!expressionStuck.empty()) {
                e1 = expressionStuck.top();
            }
            if ((expressionStuck.size() != 1)) {
                expressionStuck.pop();
                e2 = expressionStuck.top();
                expressionStuck.pop();
                expressionStuck.push(new Plus(e2, e1));
            } else {
                throw "could'nt calculate";
            }
            break;
        case '-':
            outputOueue.pop();
            e1 = expressionStuck.top();
            if ((expressionStuck.size() != 1)) {
                expressionStuck.pop();
                e2 = expressionStuck.top();
                expressionStuck.pop();
                expressionStuck.push(new Minus(e2, e1));
            } else {
                expressionStuck.push(new UMinus(e1));
            }
            break;
        case '*':
            outputOueue.pop();
            e1 = expressionStuck.top();
            expressionStuck.pop();
            e2 = expressionStuck.top();
            expressionStuck.pop();
            expressionStuck.push(new Mul(e2, e1));
            break;
        case '/':
            outputOueue.pop();
            e1 = expressionStuck.top();
            expressionStuck.pop();
            e2 = expressionStuck.top();
            expressionStuck.pop();
            expressionStuck.push(new Div(e2, e1));
            break;
        case '#':
            outputOueue.pop();
            e1 = expressionStuck.top();
            expressionStuck.pop();
            expressionStuck.push(new UPlus(e1));
            break;
        case '$':
            outputOueue.pop();
            e1 = expressionStuck.top();
            expressionStuck.pop();
            expressionStuck.push(new UMinus(e1));
            break;
    }
}

void Interpreter::makeExpressionVar(char token) {
    char token2 = token;
    int flag = 1, flag2 = 1;
    outputOueue.pop();
    if (outputOueue.size() > 0) {
        token = outputOueue.front();
    } else {
        flag = 0;
    }
    string combinesChars;
    while (flag2 == 1) {
        if ((flag == 1) && (!(token == ',') && !(this->checkIfOperator(token)))) {
            combinesChars = std::string(1, token2) + token;
            flag = 0;
            this->outputOueue.pop();
            token = this->outputOueue.front();
        } else if ((token == ',') || (this->checkIfOperator(token))) {
            combinesChars = combinesChars + token2;
            flag2 = 0;
        } else {
            combinesChars = combinesChars + token;
            if (outputOueue.size() > 0) {
                this->outputOueue.pop();
            }
            if (this->outputOueue.size() == 0) {
                flag2 = 0;
                continue;
            }
            token = this->outputOueue.front();
        }
        if ((token == ',') || this->checkIfOperator(token)) {
            flag2 = 0;
        }
    }
    // converting to value
    double d = atof(combinesChars.c_str());
    this->expressionStuck.push(new Value(d));
}

void Interpreter::makeExpressionVariable(char token) {
    char token2 = token;
    string combinesChars;
    int flag = 1, flag2 = 1;
    outputOueue.pop();
    if (!outputOueue.empty()) {
        token = outputOueue.front();
    } else {
        combinesChars = token2;
        flag2 = 0;
    }
    while (flag2 == 1) {
        if ((flag == 1) && (!(token == ',') && !(this->checkIfOperator(token)))) {
            combinesChars = std::string(1, token2) + token;
            flag = 0;
            this->outputOueue.pop();
            token = this->outputOueue.front();
        } else if ((token == ',') || (this->checkIfOperator(token))) {
            combinesChars = combinesChars + token2;
            flag2 = 0;
        } else {
            combinesChars = combinesChars + token;
            this->outputOueue.pop();
            if (!this->outputOueue.empty()) {
                token = this->outputOueue.front();
            } else {
                flag2 = 0;
            }
        }
        if ((token == ',') || this->checkIfOperator(token)) {
            flag2 = 0;
        }
    }
    // searching for the value
    // checking if the variable exist
    map<string, double>::iterator it;
    it = this->map1.find(combinesChars);
    if (it != this->map1.end()) {
        //found this name
        this->expressionStuck.push(new Variable(combinesChars, map1[combinesChars]));
    } else {
        // didnt find the name
        cout << "variable wasn't initialized- shunting yard" << endl;
    }
}

bool Interpreter::isNameValid(string s) {
    for (unsigned int i = 0; i < s.length(); i++) {
        if (i == 0) {
            if (isalpha(s[i]) == 0) {
                // checking that the first char is a letter
                return false;
            }
        }
        if (isdigit(s[i]) == 0 && s[i] != '_' && isalpha(s[i]) == 0) {
            return false;
        }
    }
    return true;
}

Interpreter::~Interpreter() {
}

float ShuntingYard::useShuntingYard(string *s, unordered_map<string, Var *> *varTable) {
    string see = *s;
    double x;
    Interpreter *interpret = new Interpreter();
    Expression *ex1 = nullptr;
    try {
        interpret->setVariables(varTable);
        ex1 = interpret->interpret(*s);
        x = ex1->calculate();
        delete ex1;
    } catch (const char *e) {
        if (ex1 != nullptr) {
            delete ex1;
        }
        std::cout << "error in shunting yard. " << *s << std::endl;
    }
    delete (interpret);
    return x;
}