//
// Created by oem on 18/12/2019.
//
//
// Created by nikol on 04/11/2019.
//

#ifndef EX1_EX1_H
#define EX1_EX1_H

#include <string>
#include <stack>
#include <queue>
#include "Expression.h"
#include "Var.h"
#include <map>
#include <unordered_map>

//
// Created by nikol on 04/11/2019.
//
class UnaryOperator : public Expression {
public:
    UnaryOperator(Expression *expression);

    virtual ~UnaryOperator();

    Expression *getExpression() const;

private:

    Expression *expression1;
public:
    double calculate();

    void setExpression(Expression *expression);
};

class UPlus : public UnaryOperator {
public:
    UPlus(Expression *right, Expression *left);

    UPlus(Expression *expression);

    double calculate();

    virtual ~UPlus();
};

class UMinus : public UnaryOperator {
public:
    UMinus(Expression *expression);

    double calculate();

    virtual ~UMinus();
};

class BinaryOperator : public Expression {

public:

    BinaryOperator(Expression *left, Expression *right);

    virtual ~BinaryOperator();

    Expression *getExpression() const;

    Expression *getRight() const;

    Expression *getLeft() const;

private:
    Expression *left1;
    Expression *right1;

};

class Plus : public BinaryOperator {
public:
    Plus(Expression *right, Expression *left);

    double calculate();

    virtual ~Plus();
};

class Minus : public BinaryOperator {
public:
    Minus(Expression *right, Expression *left);

    double calculate();

    virtual ~Minus();
};

class Mul : public BinaryOperator {
public:
    Mul(Expression *right, Expression *left);

    double calculate();

    virtual ~Mul();
};

class Div : public BinaryOperator {
public:
    Div(Expression *right, Expression *left);

    double calculate();

    virtual ~Div();
};

class Value : public Expression {
public:
    Value(double val);

private:
    double const val1;
public:
    double calculate();

    virtual ~Value();
};

class Variable : public Expression {
private:
    string name1;
    double value1;
public:
    double calculate();

    Variable(const string &name, double value);

    Variable &operator++(int);

    Variable &operator--(int);

    Variable &operator++();

    Variable &operator--();

    Variable &operator+=(double num);

    Variable &operator-=(double num);

    void setVariable(double value);

    double getVal() const;

    virtual ~Variable();

    const string &getName() const;
};

class Interpreter {
private:
    map<string, double> map1;
    stack<char> operatorStuck;
    stack<Expression *> expressionStuck;
    queue<char> outputOueue;
public:
    void setVariables(string s);

    void setVariables(std::unordered_map<string, Var *> *varTable);

    Expression *interpret(string s);

    void readAndAllocate(string s);

    int checkIfOperator(char sub);

    void manageOperators(char enterOp, char previous);

    int precedence(char enter, char stuck, char previous);

    void moveOperators();

    Expression *solve();

    virtual ~Interpreter();

    void makeExpressionOperator(char token);

    void makeExpressionVar(char token);

    void makeExpressionVariable(char token);

    bool isNumber(string s);

    bool isNameValid(string s);
};

class ShuntingYard {
public:
    static float useShuntingYard(string *s, unordered_map<string, Var *> *varTable);
};

#endif //EX1_EX1_H