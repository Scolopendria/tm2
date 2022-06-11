#ifndef ATTRIBUTECONTAINERS_CPP
#define ATTRIBUTECONTAINERS_CPP

#include "stringContainers.h"
#include <vector>
#include <string>
#include <array>

attributeContainer::attributeContainer(){
    // do nothing
}

attributeContainer::attributeContainer(std::vector<std::array<std::string, 2>> attributes){
    this->attributes = attributes;
}

std::vector<std::array<std::string, 2>> attributeContainer::getList(){
    return this->attributes;
}

std::string attributeContainer::get(std::string identifier){
    for (auto&& ID_pair : this->attributes){
        if (ID_pair[0] == identifier) return ID_pair[1];
    }
    return "NULL";
}

attributeContainer* attributeContainer::set(std::array<std::string, 2> ID_pair){
    deleteAttribute(ID_pair[0]);
    this->attributes.push_back(ID_pair);
    return this;
}

attributeContainer* attributeContainer::set(std::string identifier, std::string value){
    return set(std::array<std::string, 2>{identifier, value});
}

attributeContainer* attributeContainer::set(std::vector<std::array<std::string, 2>> attributes){
    for (auto&& ID_pair : attributes){
        set(ID_pair);
    }
    return this;
}

attributeContainer* attributeContainer::deleteAttribute(std::string identifier){
    for (std::size_t iter{this->attributes.size() - 1};  iter != SIZE_MAX; iter--){
        if (this->attributes[iter][0] == identifier) this->attributes.erase(this->attributes.begin() + iter);
    }
    return this;
}

#endif