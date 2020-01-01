#include "Var.h"
#include <string>
#include <mutex>

std::mutex mutex_lock;

/** a var has a name, sim, direction and a value.
 * we use the value of the var in order to know the values of the vars defined in the
 * text file.
 * if the direction in towards outside we will want to update the flight simulator according to the
 * values of the vars we created in our program.
 * if the direction in towards inside we will get the value of the var from the flight simulator.
 * we might create a var and want to assign a value we choose, in this case we will see the equals sign
 * and use the shunting yard in order to calculate it.
 */
Var::Var(string nameIn, string simIn, string inoutIn) {
    this->name = nameIn;
    if (inoutIn == "->") {
        this->inout = "OUT";
    } else if (inoutIn == "<-") {
        this->inout = "IN";
    } else {
        // in cases we just want to create a variable and initialize it
        this->inout = "";
    }
    this->sim = simIn;
}

float Var::getValue() {
    return this->value;
}

void Var::setValue(float val) {
    mutex_lock.lock();
    this->value = val;
    mutex_lock.unlock();
}

string Var::getArrow(){
    return this->inout;
}

string Var::getSim(){
    return this->sim;
}