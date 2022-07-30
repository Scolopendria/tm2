#ifndef LISTCONTAINERS_CPP
#define LISTCONTAINERS_CPP

#include "stringContainers.h"

li::li(std::string name){
    this->name = name;
}

std::string li::getName() const {
    return this->name;
}

std::vector<std::string> li::getValues() const {
    return this->values;
}

std::string li::getValue(std::size_t iterator) const {
    if (iterator >= this->values.size()) return "NULL";
    return this->values[iterator];
}

li* li::set(std::string value){
    this->deleteValue(value);
    this->values.push_back(value);
    return this;
}

li* li::set(std::vector<std::string> values){
    for (auto &&value : values){
        this->set(value);
    }
    return this;
}

li* li::deleteValue(std::string value){
    for (std::size_t iter{this->values.size()-1};  iter != SIZE_MAX; iter--){
        if (this->values[iter] == value){
            this->values.erase(this->values.begin() + iter);
        }
    }
    return this;
}


listContainer::listContainer(){
    return;
}

std::vector<li> listContainer::getLists() const {
    return this->lists;
}

li listContainer::get(std::string identifier) const {
    for (const auto list__ : this->lists){
        if (list__.getName() == identifier){
            return list__;
        }
    }
    return li{""};
}

listContainer* listContainer::set(std::string identifier, std::vector<std::string> values){
    for (auto &&list__ : this->lists){
        if (list__.getName() == identifier){
            list__.set(values);
            return this;
        }
    }
    return this;
}

listContainer* listContainer::set(std::string identifier, std::string value){
    this->set(identifier, std::vector<std::string>{value});
    return this;
}

listContainer* listContainer::createList(std::string identifier){
    this->deleteList(identifier);
    this->lists.push_back(li{identifier});
    return this;
}

listContainer* listContainer::deleteList(std::string identifier){
    for (std::size_t iter{this->lists.size()-1}; iter != SIZE_MAX; iter--){
        if (this->lists[iter].getName() == identifier){
            this->lists.erase(this->lists.begin() + iter);
        }
    }
    return this;
}

#endif