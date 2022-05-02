#ifndef CHILDSTRING_CPP
#define CHILDSTRING_CPP

#include "stringContainers.h"

node::node(std::string data){
    this->data = data;
}

std::string node::refresh(){
    return this->data;
}

#endif