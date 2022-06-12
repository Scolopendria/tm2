#ifndef METACONTAINERS_CPP
#define METACONTAINERS_CPP

#include "stringContainers.h"
//converters??

metaContainer::metaContainer(node data, std::string parent){
    this->name = data.getName();
    this->fullname = parent + this->name;
    this->attributes = data.attributes;
    this->timeUsed = std::stoi(this->attributes.get("time")) + std::stoi(this->attributes.get("timeMarginEnd"));
    // *WARNING* No value check (MINVH)
    for (auto child : data.getChildren()){
        if (child.attributes.get("freeRadiacals") == "true"){// pseudocheck
            this->freeRadicals.push_back(metaContainer(child, this->fullname));
        } else {
            this->children.push_back(metaContainer(child, this->fullname));
        }
    }

    this->updateTotalTime();
}

metaContainer::metaContainer(node data){
    metaContainer(data, "");
}

std::string metaContainer::getName(){
    return this->name;
}

int metaContainer::updateTotalTime(){
    this->totalTime = this->timeUsed;

    for (auto child : this->children){
        this->totalTime += child.updateTotalTime();
    }

    return this->totalTime;
}

task metaContainer::getTask(){
    return this->t;
}


metaContainer* metaContainer::initTask(int start){
    this->t = task{this->name, start, start + this->timeUsed};
    return this;
}


std::vector<metaContainer> metaContainer::extract(){// consider moving to converters or scheduler
    std::vector<metaContainer> completeList{*this};
    completeList.insert(completeList.end(), this->freeRadicals.begin(), this->freeRadicals.end());

    for (auto &&child : this->children){
        const std::vector<metaContainer> radicalList{child.extractFreeRadicals()};
        completeList.insert(completeList.end(), radicalList.begin(), radicalList.end());
    }

    for (auto &&child : this->freeRadicals){
        const std::vector<metaContainer> radicalList{child.extractFreeRadicals()};
        completeList.insert(completeList.end(), radicalList.begin(), radicalList.end());
    }

    return completeList;
}

std::vector<metaContainer> metaContainer::extractFreeRadicals(){
    std::vector<metaContainer> radicalList{this->freeRadicals};

    for (auto &&child : this->children){
        const std::vector<metaContainer> pList{child.extractFreeRadicals()};
        radicalList.insert(radicalList.end(), pList.begin(), pList.end());
    }

    for (auto &&child : this->freeRadicals){
        const std::vector<metaContainer> pList{child.extractFreeRadicals()};
        radicalList.insert(radicalList.end(), pList.begin(), pList.end());
    }

    return radicalList;
}

#endif