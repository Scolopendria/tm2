#ifndef METACONTAINERS_CPP
#define METACONTAINERS_CPP

#include "stringContainers.h"
#include <algorithm>

metaContainer::metaContainer(node data, std::string parent){
    this->name = data.getName();
    this->fullname = parent + this->name;
    this->attributes = data.attributes;
    this->timeUsed = std::stoi(this->attributes.get("time")) + std::stoi(this->attributes.get("timeMarginEnd"));
    // *WARNING* No value check (MINVH)
    // consider whether to move radical tasks during initialization or later // maybe not
    for (auto &&child : data.getChildren()){
        this->children.push_back(metaContainer(child, this->fullname));
    }

    if (this->name != "self" && this->attributes.get("shellCast") != "true") this->children.push_back(metaContainer{this->fullname, this->attributes});
    this->updateTotalTime();
}

metaContainer::metaContainer(node data){
    metaContainer(data, "");
}

metaContainer::metaContainer(std::vector<metaContainer> day){
    this->attributes.set("shellCast", "true");
    this->t = task{"", 0, 1440};

    for (auto &&child : day){
        this->children.push_back(child);
    }

    this->updateTotalTime();
}

metaContainer::metaContainer(std::string fullname, attributeContainer inheritAttributes){
    this->name = "self";
    this->fullname = fullname;
    this->attributes = inheritAttributes;
    this->timeUsed = std::stoi(this->attributes.get("time")) + std::stoi(this->attributes.get("timeMarginEnd"));
    // *WARNING* No value check (MINVH)
}

std::string metaContainer::getName(){
    return this->name;
}

std::string metaContainer::getFullname(){
    return this->fullname;
}

int metaContainer::updateTotalTime(){
    if (this->name == "self"){
        totalTime == timeUsed;
    }

    for (auto &child : this->children){// fix
        this->totalTime += child.updateTotalTime();
    }

    return this->totalTime;
}

task metaContainer::getTask(){
    return this->t;
}


metaContainer* metaContainer::init(int childPosition, int start){
    // initialize task
    this->children[childPosition].t = task{this->name, start, start + this->updateTotalTime()};
    // add to scheduled
    this->scheduledChildren.push_back(this->children[childPosition]);
    // delete from unscheduled
    this->children.erase(this->children.begin() + childPosition);

    std::sort(
        this->scheduledChildren.begin(),
        this->scheduledChildren.end(),
        [](metaContainer a, metaContainer b){
            return a.getTask().getStart() < b.getTask().getStart();
        }
    );

    return this;
}

metaContainer* metaContainer::uninit(int childPosition){// undefined
    return this;
}


metaContainer metaContainer::extract(){// consider moving to converters or scheduler
    std::vector<metaContainer> completeList{};

    for (std::size_t i{this->children.size()}; i != SIZE_MAX; i--){
        const std::vector<metaContainer> radicals{this->children[i].extractFreeRadicals()};
        completeList.insert(completeList.end(), radicals.begin(), radicals.end());

        if (this->children[i].attributes.get("freeRadical") == "true"){
            completeList.push_back(this->children[i]);
            this->children.erase(this->children.begin() + i);
        }
    }

    completeList.push_back(*this);
    return metaContainer{completeList};
}

std::vector<metaContainer> metaContainer::extractFreeRadicals(){
    std::vector<metaContainer> radicalList{};

    for (std::size_t i{this->children.size()}; i != SIZE_MAX; i--){
        const std::vector<metaContainer> radicals{this->children[i].extractFreeRadicals()};
        radicalList.insert(radicalList.end(), radicals.begin(), radicals.end());
        
        if (this->children[i].attributes.get("freeRadical") == "true"){
            radicalList.push_back(this->children[i]);
            this->children.erase(this->children.begin() + i);
        }
    }

    return radicalList;
}

#endif