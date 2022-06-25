#include <iostream>
#include <string>
#include <vector>
#include "include/headers.h"
//only non-hDev component: node.cpp:refresh(), metacontainer.cpp:updateTotalTime()
//and some old order functions

int main(int argc, char **argv){
    megaString tm2{[argc, argv](){
        std::vector<std::string> commands{};
        for (int i{}; i < argc; i++){
            commands.push_back(argv[i]);
        }
        commands.erase(commands.begin());
        return commands;
    }()};
    tm2 = scheduler(tm2);
    
    return 0;
}