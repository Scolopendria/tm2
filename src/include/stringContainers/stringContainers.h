#ifndef STRINGCONTAINERS_H
#define STRINGCONTAINERS_H

#include "../headers.h"
#include <string>
#include <vector>
#include <array>

class calTime{
    public:
        calTime();
        calTime(int offset);
        calTime* initialize(int offset);
        int year{};
        int month{};
        int date{};
        int weekday{};
        int minute_t{};
        std::string strDate{};
        std::string baseDate{};
};

//utilities?
class task{// any redundant members?
    private:
        int startTime{};
        int endTime{};
        int timeUsed{};
        std::string name{};
        std::string stdTime{};
    public:
        task(std::string taskName, int start, int end);
        int getStart() const;
        int getEnd() const;
        int getTimeUsed() const;
        std::string getName() const;
        std::string getFullStdTime() const;
};

class attributeContainer{
    private:
        std::vector<std::array<std::string, 2>> attributes{};
    public:
        attributeContainer();
        attributeContainer(std::vector<std::array<std::string, 2>> attributes);
        std::vector<std::array<std::string, 2>> getList() const;
        std::string get(std::string identifier) const;
        attributeContainer* set(std::array<std::string, 2> ID_pair);
        attributeContainer* set(std::string identifier, std::string value);// Overload
        attributeContainer* set(std::vector<std::array<std::string, 2>> attributes);
        attributeContainer* deleteAttribute(std::string identifier);
};

class li{
    private:
        std::string name{};
        std::vector<std::string> values;
    public:
        li(std::string name);
        std::string getName() const;
        std::string getValue(std::size_t iterator) const;
        std::vector<std::string> getValues() const;
        li* set(std::string value);
        li* set(std::vector<std::string> values); 
        li* deleteValue(std::string value);
};

class listContainer{
    private:
        std::vector<li> lists{};
    public:
        listContainer();
        li get(std::string identifier) const;
        std::vector<li> getLists() const;
        listContainer* set(std::string identifier, std::vector<std::string> values);
        listContainer* set(std::string identifier, std::string value);// Overload
        listContainer* createList(std::string identifier);
        listContainer* deleteList(std::string identifier);
};

class node{
    private:
        std::string data{};
        std::string name{};
        std::vector<node> children{};
        node* objectify();
    public:
        node();
        node(std::string data);
        attributeContainer attributes{};
        listContainer lists{};
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
        std::string name{};
        std::string fullname{};
        task t{"", 0, 0};
        std::vector<metaContainer> extractFreeRadicals();
    public:
        metaContainer(node data, calTime tempor, std::string parent, attributeContainer attributes, listContainer lists);
        metaContainer(std::string parent, attributeContainer attributes, listContainer lists);
        metaContainer(std::vector<node> day, calTime tempor);
        std::vector<metaContainer> children{};
        std::vector<metaContainer> scheduledChildren{};
        attributeContainer attributes{};
        listContainer lists{};
        std::string getName() const;
        std::string getFullname() const;
        task getTask() const;
        int getTotalTime() const;
        int updateTotalTime();
        metaContainer* initialize(std::size_t childPosition, int start);
        metaContainer* uninitilize(std::size_t childPosition);
        metaContainer* extract();// utilities?
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
#include "listContainers.cpp"
#include "metaContainers.cpp"
#include "schedulerClasses.cpp"

#endif