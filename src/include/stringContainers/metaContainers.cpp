#ifndef METACONTAINERS_CPP
#define METACONTAINERS_CPP

#include "stringContainers.h"

metaContainer::metaContainer(node data){
    this->name = data.getName();
    this->attributes = data.attributes;
    totalTime = std::stoi(this->attributes.get("time")) + std::stoi(this->attributes.get("timeMarginEnd"));
    // *WARNING* No value check

    for (auto child : data.getChildren()){
        this->children.push_back(metaContainer(child));
    }

    for (auto child : children){
        totalTime += child.getTotalTime();
    }
}

int metaContainer::getTotalTime(){
    return this->totalTime;
}

std::string metaContainer::getName(){
    return this->name;
}

#endif