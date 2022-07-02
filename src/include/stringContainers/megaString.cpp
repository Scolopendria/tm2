#ifndef MEGASTRING_CPP
#define MEGASTRING_CPP

#include "stringContainers.h"
#include <string>
#include <vector>
#include <fstream>
#include <sstream>


////// converters /////////////////////////
// old-order functions
std::string read(std::string s, std::string::size_type &i, bool t){
    std::string identifier;
    if (t) identifier += s[i];
    i++;
    while (i < s.length() && s[i] != '"') identifier += s[i++];
    if (t) identifier += s[i];
    i++;
    return identifier;
}

std::string formattedData(std::string data){
    bool gate{false};
    int depth{};
    std::string str{};
    std::string::size_type i{};
    
    while(i < data.length()){
        switch (data[i]){
            case '"': str += read(data, i, true);
            if (gate){
                str += '\n';
                for (int ctr = 0; ctr < depth; ctr++) str += '\t';
                gate = false;
            }
            break;
            case '{': str += "{\n"; i++; depth++;
            for (int ctr = 0; ctr < depth; ctr++) str += '\t';
            break;
            case '}': str.pop_back(); str += "}\n"; i++; depth--;
            for (int ctr = 0; ctr < depth; ctr++) str += '\t';
            break;
            case '=': str += data[i++]; gate = true;
            break;
            default: i++;
        }
    }

    return str;
}

////////////////////////////////

megaString::megaString(std::vector<std::string> commands){
    this->filename = commands[0];
    commands.erase(commands.begin());

    this->child = node{strip(getFile(filename))};

    //commands();
}

megaString::~megaString(){
    std::ofstream file(filename);
    file << formattedData(this->child.refresh());
    file.close();
}

std::string megaString::getFile(const std::string filename){// utilities
    std::ifstream file(filename);
    if (!file.is_open()) return "\"tm2\"{}";

    std::ostringstream ss{};
    ss << file.rdbuf();
    file.close();

    if (ss.str().find_first_not_of(" \t\n\v\r") != std::string::npos) return ss.str();
    return "\"tm2\"{}";
}

std::string megaString::strip(const std::string data){// place in converters?? sub of validateFile
    // MINVH
    // scoping not complete
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

            str += data[i++];
            break;
        case '{':
            str += data[i++];
            depth++;
            break;
        case '}':
            str += data[i++];
            depth--;
            if (!depth) return str;
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