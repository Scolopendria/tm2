#ifndef STRINGCONTAINERS_H
#define STRINGCONTAINERS_H

#include "../headers.h"
#include <string>
#include <vector>
#include <array>

//utilities?
class task{// any redundant members?
    private:
        int startTime;
        int endTime;
        int timeUsed;
        std::string name;
        std::string stdTime;
    public:
        task(std::string taskName, int start, int end);
        int getStart();
        int getEnd();
        int getTimeUsed();
        std::string getName();
        std::string getFullStdTime();
};

class attributeContainer{
    private:
        std::vector<std::array<std::string, 2>> attributes{};
    public:
        attributeContainer();
        attributeContainer(std::vector<std::array<std::string, 2>> attributes);
        std::vector<std::array<std::string, 2>> getList();
        std::string get(std::string identifier);
        attributeContainer* set(std::array<std::string, 2> ID_pair);
        attributeContainer* set(std::string identifier, std::string value);// Overload
        attributeContainer* set(std::vector<std::array<std::string, 2>> attributes);
        attributeContainer* deleteAttribute(std::string identifier);
};

class node{
    private:
        std::string data{};
        std::string name{};
        std::vector<node> children{};
        node* objectify();// a bit of a mess
    public:
        node();
        node(std::string data);
        attributeContainer attributes{};
        std::string refresh();
        std::string getName();
        std::vector<node> getChildren();
        node* deleteChild(std::string childIdentifier);
        node* forge(node child);
        node* forge(std::string childData);
        node* forge(std::vector<node> children);
        node get(std::string childName);
        bool check(std::string childIdentifier);
};

class metaContainer{
    private:
        int totalTime{};
        int timeUsed{};// redundant?
        std::string name{};
        std::string fullname{};
        task t{std::string{}, int{}, int{}};
        std::vector<metaContainer> extractFreeRadicals();
    public:
        metaContainer(node data, std::string parent);
        metaContainer(node data);// Overload
        metaContainer(std::string fullname, attributeContainer inheritAttributes);
        metaContainer(std::vector<metaContainer> day);
        std::vector<metaContainer> children{};
        std::vector<metaContainer> scheduledChildren{};
        attributeContainer attributes{};
        std::string getName();
        std::string getFullname();
        task getTask();
        int updateTotalTime();
        metaContainer* init(std::size_t childPosition, int start);
        metaContainer* uninit(std::size_t childPosition);
        metaContainer extract();// utilities?
};

class megaString{
    private:
        std::string filename{};
        std::string getFile(std::string filename);
        std::string strip(std::string data);
    public:
        megaString(std::vector<std::string> commands);
        ~megaString();
        node child{};
};

#include "megaString.cpp"
#include "node.cpp"
#include "attributeContainers.cpp"
#include "metaContainers.cpp"
#include "schedulerClasses.cpp"

#endif