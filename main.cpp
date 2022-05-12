#include <iostream>
#include "getting_started/tutorial.h"

// number of args, each of which is pointer to char* (a string)
int main (int argc, char* args[]) {
    std::cout << "Hello world" << std::endl;
    Tutorial t;
    return t.RunTutorial();
}