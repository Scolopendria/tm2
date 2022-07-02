#ifndef METACONTAINERS_CPP
#define METACONTAINERS_CPP

#include "stringContainers.h"
#include <algorithm>
#include <iostream>

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
    
    for (auto &&child : data.getChildren()){
        this->children.push_back(metaContainer(child, this->fullname));
    }

    if (this->name != "self" && this->attributes.get("shellCast") != "true"){
        this->children.push_back(metaContainer{this->fullname, this->attributes});
    }

    this->updateTotalTime();
}

metaContainer::metaContainer(node data){
    *this = metaContainer(data, "");
}

metaContainer::metaContainer(std::vector<node> day, int currentTime){
    this->name = "";
    this->fullname = "day";
    this->attributes.set("shellCast", "true");
    this->t = task{"", currentTime, 1440};

    for (auto &&child : day){
        this->children.push_back(metaContainer{child});
    }

    this->updateTotalTime();
}

metaContainer::metaContainer(std::string fullname, attributeContainer inheritAttributes){
    this->name = "self";
    this->fullname = fullname;
    this->attributes = inheritAttributes;
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

task metaContainer::getTask(){
    return this->t;
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

metaContainer* metaContainer::initialize(std::size_t childPosition, int start){
    //no guarantee scheduling, kick from lowest priority
    int upperBound{};

    // initialize task
    if (this->children[childPosition].getName() == "self"){
        this->children[childPosition].t =
            task{
                this->children[childPosition].fullname,
                start,
                start + superstoi(this->children[childPosition].attributes.get("time"), 30)
            };
    } else {
        this->children[childPosition].t =
            task{this->children[childPosition].fullname,
            start,
            start + this->children[childPosition].updateTotalTime()
        };
    }

    // add to scheduled and delete from unscheduled
    this->scheduledChildren.push_back(this->children[childPosition]);
    this->children.erase(this->children.begin() + childPosition);

    std::sort(
        this->scheduledChildren.begin(),
        this->scheduledChildren.end(),
        [](metaContainer a, metaContainer b){
            return a.getTask().getStart() < b.getTask().getStart();
        }
    );

    // optimize free space // utilities??
    // todo: optimize order based on time length and order (after) magnetism

    std::vector<int> boundedChildrenList;
    for (std::size_t i{}; i < this->scheduledChildren.size(); i++){
        if (this->scheduledChildren[i].attributes.get("start") != "NULL"){
            boundedChildrenList.push_back(i);
        }
    }

    if (boundedChildrenList.empty()){
        upperBound = this->getTask().getStart();
        for (std::size_t i{}; i < this->scheduledChildren.size(); i++){
            if (// equivalent to the line that is 12 lines after this line (line 224)
                upperBound + this->scheduledChildren[i].getTask().getTimeUsed()+1 >
                this->getTask().getEnd()
            ) {
                this->uninitilize(i--);
                continue;
            }

            this->scheduledChildren[i].t =
                task{
                    this->scheduledChildren[i].getTask().getName(),
                    upperBound,
                    upperBound + this->scheduledChildren[i].getTask().getTimeUsed()
                };
            
            upperBound = this->scheduledChildren[i].getTask().getEnd()+1;
        }
        return this;
    }

    upperBound = this->getTask().getStart();
    for (std::size_t i{}; i < boundedChildrenList.front(); i++){
        if (
            upperBound + this->scheduledChildren[i].getTask().getTimeUsed()+1 >
            this->scheduledChildren[boundedChildrenList.front()].getTask().getStart()
        ) {
            this->uninitilize(i--);
            continue;
        }

        this->scheduledChildren[i].t =
            task{
                this->scheduledChildren[i].getTask().getName(),
                upperBound,
                upperBound + this->scheduledChildren[i].getTask().getTimeUsed()
            };
        
        upperBound = this->scheduledChildren[i].getTask().getEnd()+1;
    }

    upperBound = this->scheduledChildren[boundedChildrenList.back()].getTask().getEnd()+1;
    for (std::size_t i{boundedChildrenList.back()+(std::size_t)1}; i < this->scheduledChildren.size(); i++){
        if (
            upperBound + this->scheduledChildren[i].getTask().getTimeUsed()+1 >
            this->getTask().getEnd()
        ) {
            uninitilize(i--);
            continue;
        }

        this->scheduledChildren[i].t =
            task{
                this->scheduledChildren[i].getTask().getName(),
                upperBound,
                upperBound + this->scheduledChildren[i].getTask().getTimeUsed()
            };
        
        upperBound = this->scheduledChildren[i].getTask().getEnd()+1;
    }

    if (boundedChildrenList.size() != 1){
        for (std::size_t i{}; i < boundedChildrenList.size()-2; i++){
            upperBound = this->scheduledChildren[i].getTask().getEnd()+1;
            for (std::size_t j{boundedChildrenList[i]+(std::size_t)1}; j < boundedChildrenList[i+1]; j++){
                if (
                    upperBound + this->scheduledChildren[j].getTask().getTimeUsed()+1 >
                    this->scheduledChildren[boundedChildrenList[i+1]].getTask().getStart()
                ) {
                    uninitilize(j--);
                    continue;
                }

                this->scheduledChildren[j].t =
                    task{
                        this->scheduledChildren[j].getTask().getName(),
                        upperBound,
                        upperBound + this->scheduledChildren[j].getTask().getTimeUsed()
                    };
                
                upperBound = this->scheduledChildren[j].getTask().getEnd()+1;
            }
        }
    }

    return this;
}

metaContainer* metaContainer::uninitilize(std::size_t childPosition){
    this->scheduledChildren[childPosition].t = task{this->name, 0, 0};
    this->children.push_back(this->scheduledChildren[childPosition]);
    this->scheduledChildren.erase(this->scheduledChildren.begin() + childPosition);
    return this;
}


metaContainer* metaContainer::extract(){// consider moving to converters or scheduler
    std::vector<metaContainer> completeRadicalList{};

    for (std::size_t i{this->children.size()-1}; i != SIZE_MAX; i--){
        const std::vector<metaContainer> radicalList = this->children[i].extractFreeRadicals();
        completeRadicalList.insert(completeRadicalList.end(), radicalList.begin(), radicalList.end());

        if (this->children[i].attributes.get("freeRadical") == "true"){
            completeRadicalList.push_back(this->children[i]);
            this->children.erase(this->children.begin() + i);
        }
    }

    for (auto &&child : completeRadicalList){
        this->children.push_back(child);
    }

    this->updateTotalTime();

    return this;
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