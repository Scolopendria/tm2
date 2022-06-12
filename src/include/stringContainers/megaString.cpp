#ifndef MEGASTRING_CPP
#define MEGASTRING_CPP

#include "stringContainers.h"
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

megaString::megaString(std::vector<std::string> commands){
    this->filename = commands[0];
    commands.erase(commands.begin());

    this->child = node{strip(getFile(filename))};

    //commands();
}

megaString::~megaString(){
    std::ofstream file(filename);
    file << this->child.refresh();
    file.close();
}

std::string megaString::getFile(const std::string filename){ // utilities
    std::ifstream file(filename);
    if (!file.is_open()) return "\"tm2\"{}";

    std::ostringstream ss{};
    ss << file.rdbuf();
    file.close();

    if (ss.str().find_first_not_of(" \t\n\v\r") != std::string::npos) return ss.str();
    return "\"tm2\"{}";
}

std::string megaString::strip(const std::string data){// place in converters?? sub of validateFile
    //MINVH
    //scoping not complete
    // does not guarantee objectify valid data
    int depth{};
    std::string str{};
    std::size_t i{};
    
    while (i < data.length()){
        switch (data[i]){
        case '"':
            str += data[i++];
            while(i < data.length()){
                if (data[i] != '"') str += data[i++];
                else break;
            }
            if (i == data.length()) /*throwError("File Overran")*/;
            // Does not handle special characters very well

            str += data[i++];
            break;
        case '{':
            str += data[i++];
            depth++;
            break;
        case '}':
            str += data[i++];
            if (--depth < 1) return str;
            break;
        case '=':
            str += data[i++];
            break;
        default: i++;
        }
    }
    //throwError("file overran")
    return str;
}

#endif