#include <iostream>
#include <string>
#include <vector>
#include "include/headers.h"

int main(int argc, char **argv){
    // std::cout << "Hello World" << std::endl;

    std::vector<std::string> commands{};
    for (int i{}; i < argc; i++){
        commands.push_back(argv[i]);
    }
    commands.erase(commands.begin());

    // for (const std::string argument : commands){
    //     std::cout << argument << "\n";
    // }

    return 0;
}

/*
bool ctr{false}, gate[2]{true, true};
    std::string str[2];
    verStr* currentLocation;
    
    // strSwitch switcher{func};
    // switcher.add(switchPair{"bye", command});
    // switcher.match("bye");
    // add a switch class and command style format
    while (gate[0]){
        gate[1] = true;
        std::cout << "Your name: ";
        std::cin >> str[0];
        if (str[0] == "") gate[1] = 0;
        else currentLocation = mStr->vStr.child(str[0], ctr)->child("Goals", ctr);

        while (gate[1]){
            std::cout << "Enter command: ";
            std::cin >> str[0];
            if (str[0] == "!quit" || str[0] == ""){
                gate[1] = false;
                gate[0] = false;
            } else if (str[0] == "!logout"){
                gate[1] = false;
            } else if (str[0] == "!attribute"){
                std::cin >> str[0] >> str[1];
                currentLocation->attribute(str[0], str[1]);
            } else if (str[0] == "!child"){
                std::cin >> str[0];
                if (str[0] == "check"){
                    ctr = true;
                    std::cin >> str[0];
                    currentLocation->child(str[0], ctr);
                    if (ctr){
                        std::cout << "Task: '" << str[0] << "' exists." << std::endl;
                        ctr = false;
                    } else std::cout << "Task: '" << str[0] << "' does not exist." << std::endl;
                } else if (str[0] == "enter"){
                    std::cin >> str[0];
                    currentLocation = currentLocation->child(str[0], ctr);
                } else currentLocation->child(str[0], ctr);
            } else if (str[0] == "!out"){
                if (currentLocation->getName() == "Goals") std::cout << "Already at Goals." << std::endl;
                else currentLocation = currentLocation->getParent();
            } else if (str[0] == "!delete"){
                std::cin >> str[0] >> str[1];
                if (str[0] == "attribute") currentLocation->deleteAttribute(str[1]);
                else if (str[0] == "child") currentLocation->deleteChild(str[1]);
                else std::cout << "Invalid type" << std::endl;
            } else if (str[0] == "!view") std::cout << currentLocation->getView() << std::endl;
            else if (str[0] == "!help") std::cout << "Go read the source code." << std::endl;
            else std::cout << "Command does not exist. !help for more." << std::endl;
        }
    }
*/