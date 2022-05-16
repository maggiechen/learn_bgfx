#include <iostream>
#include "getting_started/LearnBgfx.h"

// number of args, each of which is pointer to char* (a string)
int main (int argc, char* args[]) {

    std::string applicationPath = args[0];
    std::cout << applicationPath << std::endl;
    std::cout << "Hello world" << std::endl;
    LearnBgfx t;
    const char* configFile = "config.json";
    return t.Run(configFile);
}