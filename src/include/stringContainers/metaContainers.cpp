#ifndef METACONTAINERS_CPP
#define METACONTAINERS_CPP

#include "stringContainers.h"
#include <algorithm>
#include <iostream>

// birng back getTotalTime()
//fix timeUsed
///// error handling ////////////////////

int superstoi(std::string value, int defaultValue){// not final form
    auto isNumber = [](const std::string& s){
        std::string::const_iterator i = s.begin();
        while (i != s.end() && std::isdigit(*i)) ++i;
        return !s.empty() && i == s.end();
    };

    if (isNumber(value)){
        return std::stoi(value);
    }
    
    return defaultValue;
}

////////////////////////

metaContainer::metaContainer(node data, std::string parent){
    this->name = data.getName();
    this->fullname = parent + ":" + this->name;
    this->attributes = data.attributes;
    // *WARNING* No value check (MINVH)
    
    for (auto &&child : data.getChildren()){
        this->children.push_back(metaContainer(child, this->fullname));
    }

    if (this->name != "self" && this->attributes.get("shellCast") != "true") this->children.push_back(metaContainer{this->fullname, this->attributes});
    this->updateTotalTime();
}

metaContainer::metaContainer(node data){
    *this = metaContainer(data, "");
}

metaContainer::metaContainer(std::vector<metaContainer> day, int currentTime){
    this->name = "";
    this->fullname = "day";
    this->attributes.set("shellCast", "true");
    this->t = task{"", currentTime, 1440};

    for (auto &&child : day){
        this->children.push_back(child);
    }

    this->updateTotalTime();
}

metaContainer::metaContainer(std::string fullname, attributeContainer inheritAttributes){
    this->name = "self";
    this->fullname = fullname;
    this->attributes = inheritAttributes;
    this->updateTotalTime();
    // *WARNING* No value check (MINVH)
}

std::string metaContainer::getName(){
    return this->name;
}

std::string metaContainer::getFullname(){
    return this->fullname;
}

int metaContainer::getTotalTime(){
    return this->totalTime;
}

int metaContainer::updateTotalTime(){
    if (this->name == "self"){
        this->totalTime =
            superstoi(this->attributes.get("time"), 30) +
            superstoi(this->attributes.get("timeMarginEnd"), 0);
        return this->totalTime;
    }
    
    this->totalTime = 0;

    for (auto &child : this->children){// upgrade to hDev
        this->totalTime += child.updateTotalTime()+1;
    }

    for (auto &child : this->scheduledChildren){
        this->totalTime += child.updateTotalTime()+1;
    }

    return this->totalTime;
}

task metaContainer::getTask(){
    return this->t;
}


metaContainer* metaContainer::init(std::size_t childPosition, int start){
    // initialize task

    //find better way to  do this

    if (this->children[childPosition].getName() == "self"){
        this->children[childPosition].t = task{this->name, start, start + superstoi(this->children[childPosition].attributes.get("time"), 30)};
    } else{
        this->children[childPosition].t = task{this->name, start, start + this->children[childPosition].updateTotalTime()};
    }

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

metaContainer* metaContainer::uninit(std::size_t childPosition){
    this->scheduledChildren[childPosition].t = task{this->name, 0, 0};
    this->children.push_back(this->scheduledChildren[childPosition]);
    this->scheduledChildren.erase(this->scheduledChildren.begin() + childPosition);
    return this;
}


metaContainer metaContainer::extract(int currentTime){// consider moving to converters or scheduler
    std::vector<metaContainer> completeList{};

    for (std::size_t i{this->children.size()-1}; i != SIZE_MAX; i--){
        const std::vector<metaContainer> radicals = this->children[i].extractFreeRadicals();
        completeList.insert(completeList.end(), radicals.begin(), radicals.end());

        if (this->children[i].attributes.get("freeRadical") == "true"){
            completeList.push_back(this->children[i]);
            this->children.erase(this->children.begin() + i);
        }
    }

    completeList.push_back(*this);
    return metaContainer{completeList, currentTime};
}

std::vector<metaContainer> metaContainer::extractFreeRadicals(){
    std::vector<metaContainer> radicalList{};

    for (std::size_t i{this->children.size()-1}; i != SIZE_MAX; i--){
        const std::vector<metaContainer> radicals = this->children[i].extractFreeRadicals();
        radicalList.insert(radicalList.end(), radicals.begin(), radicals.end());
        
        if (this->children[i].attributes.get("freeRadical") == "true"){
            radicalList.push_back(this->children[i]);
            this->children.erase(this->children.begin() + i);
        }
    }

    return radicalList;
}

#endif