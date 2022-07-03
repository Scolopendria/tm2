#ifndef METACONTAINERS_CPP
#define METACONTAINERS_CPP

#include "stringContainers.h"
#include <algorithm>
#include <chrono>
#include <random>
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

calTime::calTime(){
    std::stringstream ss{};
    auto now{std::chrono::system_clock::to_time_t(
        std::chrono::system_clock::now()
    )};
    auto ltm{std::localtime(&now)};
    this->year = 1900 + ltm->tm_year;
    this->month = 1 + ltm->tm_mon;
    this->date = ltm->tm_mday;
    ss << year << ":" << month << ":" << date;
    this->baseDate = ss.str();
};


calTime::calTime(int offset){
    *this = calTime{};
    this->initialize(offset);
}

calTime* calTime::initialize(int offset){
    std::stringstream ss{};
    auto now{std::chrono::system_clock::to_time_t(
        std::chrono::system_clock::now() + std::chrono::days(offset)
    )};
    auto ltm{std::localtime(&now)};
    this->year = 1900 + ltm->tm_year;
    this->month = 1 + ltm->tm_mon;
    this->date = ltm->tm_mday;
    this->weekday = ltm->tm_wday;
    this->minute_t = (ltm->tm_hour * 60) + ltm->tm_min;
    ss << year << ":" << month << ":" << date;
    this->strDate = ss.str();
    this->minute_t *= (!offset);
    return this;
};

metaContainer::metaContainer(node data, calTime tempor, std::string parent, attributeContainer attributes){
    this->name = data.getName();
    this->fullname = parent + ":" + this->name;
    if (parent == "") this->fullname = this->name;

    this->attributes = data.attributes;
    // set inherited and implied attributes
    // MINVH and other erroi handling
    if (this->attributes.get("date") == "NULL"){
        if (attributes.get("date") == "NULL"){
            this->attributes.set("date", tempor.baseDate);
        } else {
            this->attributes.set("date", attributes.get("date"));
        }
    }

    if (this->attributes.get("priority") == "NULL"){
        if (attributes.get("priority") == "NULL"){
            this->attributes.set("priority", "70");
        } else {
            this->attributes.set("priority", attributes.get("priority"));
        }
    }

    if (this->attributes.get("time") == "NULL"){// change to isNumber
        this->attributes.set("time", "30");
    }

    if (this->attributes.get("start") != "NULL"){// change to isNumber
        this->attributes.set("bounded", "true");
    }

    if (this->attributes.get("after") != "NULL"){
        this->attributes.set("orderBounded", "true");
    }
    ///
    
    for (auto &&child : data.getChildren()){
        this->children.push_back(metaContainer(child, tempor, this->fullname, this->attributes));
    }

    // mix up order randomly for better result distribution
    std::shuffle(
        this->children.begin(),
        this->children.end(),
        std::default_random_engine(std::chrono::system_clock::now()
            .time_since_epoch()
            .count()
        )
    );

    if (this->name != "self" && this->attributes.get("shellCast") != "true"){
        this->children.push_back(metaContainer{this->fullname, this->attributes});
    }
}

metaContainer::metaContainer(std::vector<node> day, calTime tempor){
    std::srand(std::time(nullptr));
    this->fullname = "day";
    this->t = task{"", tempor.minute_t, 1440};

    for (auto &&child : day){
        this->children.push_back(metaContainer{child, tempor, "", attributeContainer{}});
    }

    std::shuffle(
        this->children.begin(),
        this->children.end(),
        std::default_random_engine(std::chrono::system_clock::now()
            .time_since_epoch()
            .count()
        )
    );
}

metaContainer::metaContainer(std::string parent, attributeContainer attributes){
    this->name = "self";
    this->fullname = parent;
    this->attributes = attributes;

    // filter relative traits

    this->attributes.deleteAttribute("orderBounded")->deleteAttribute("after");
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
            // error checks will be handled natively during constuction
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