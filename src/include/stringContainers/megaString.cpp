#ifndef MEGASTRING_CPP
#define MEGASTRING_CPP

#include "stringContainers.h"
#include <string>
#include <fstream>
#include <sstream>

megaString::megaString(){
    megaString("tm2.txt");
}

megaString::megaString(const std::string filename){
    this->filename = filename;
    this->child = node{strip(getFile(this->filename))};
}

megaString::~megaString(){
    std::ofstream file(filename);
    file << this->child.refresh();
    file.close();
}

std::string megaString::getFile(const std::string filename){
    std::ifstream file(filename);
    if (!file.is_open()) return "\"tm2\"{}";

    std::ostringstream ss{};
    ss << file.rdbuf();
    file.close();

    if (ss.str().find_first_not_of(" \t\n\v\r") != std::string::npos) return ss.str();
    return "\"tm2\"{}";
}

std::string megaString::strip(const std::string data){
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
            if (--depth == 0) return str;
            break;
        case '=':
            str += data[i++];
            break;
        default: ;
        }
    }

    return str;
}

#endif