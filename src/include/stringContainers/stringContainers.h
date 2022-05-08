#ifndef STRINGCONTAINERS_H
#define STRINGCONTAINERS_H

#include <string>
#include <vector>
#include <array>

class attributeContainer{
    private:
        std::vector<std::array<std::string, 2>> attributes{};
    public:
        attributeContainer();
        attributeContainer(std::vector<std::array<std::string, 2>> attributes);
        std::vector<std::array<std::string, 2>> getList();
        std::string get(std::string identifier);
        attributeContainer* set(std::array<std::string, 2> ID_pair);
        attributeContainer* set(std::string identifier, std::string value); //Overload
        attributeContainer* set(std::vector<std::array<std::string, 2>> attributes);
        attributeContainer* deleteAttribute(std::string identifier);
};

class node{
    private:
        std::string data{};
        std::string name{};
        std::vector<node> children{};
        node* objectify();
    public:
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

class megaString{
    private:
        std::string filename{};
        std::string getFile(std::string filename);
        std::string strip(std::string data);
    public:
        megaString(std::vector<std::string> commands);
        ~megaString();
        node child{"\"tm2\"{}"};
};

#include "megaString.cpp"
#include "node.cpp"
#include "attributeContainers.cpp"

#endif