#include <iostream>
#include <string>
#include <vector>
#include "include/headers.h"

int main(int argc, char **argv){
    std::vector<std::string> commands{};
    for (int i{}; i < argc; i++){
        commands.push_back(argv[i]);
    }
    commands.erase(commands.begin());

    megaString tm2{commands}; // can make lambda pass
    //tm2 = scheduler(tm2);
    return 0;
}