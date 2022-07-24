#ifndef CHILDSTRING_CPP
#define CHILDSTRING_CPP

#include "stringContainers.h"
#include <string>
#include <array>
#include <vector>
#include <iostream>

node::node(){
    // do nothing
}

node::node(std::string data){
    this->data = data;
    objectify();
}

node* node::objectify(){

    class package{
        public:
            std::string id{};
            std::string::size_type i{};
            package(std::string id, std::string::size_type i){
                this->id = id;
                this->i = i;
            };
    };
    
    auto read = [](const std::string data, package pack){
        pack.id = "";
        for (pack.i++; pack.i < data.length(); pack.i++){
            if (data[pack.i] != '"') pack.id += data[pack.i];
            else break;
        }
        pack.i++;

        return pack;
    };
    
    package pack = read(this->data, package{"", 0});
    this->name = pack.id;

    pack.i++;
    while (pack.i < this->data.length() - 1){
        std::size_t pincer{pack.i};
        pack = read(this->data, pack);
        std::string id{pack.id};
        char p{this->data[pack.i]};

        pack.i++;
        if (p == '='){
            pack = read(this->data, pack);
            this->attributes.set(id, pack.id);
            continue;
        }
        
        if (p == '{'){
            int depth = 1;
            int x = data.length();
            while (pack.i < this->data.length() && depth){
                switch (this->data[pack.i]){
                    case '"': pack = read(this->data, pack);
                    break;
                    case '{': depth++; pack.i++;
                    break;
                    case '}': depth--; pack.i++;
                    break;
                    case '=':
                    case '[':
                    case ']':
                        pack.i++;
                    break;
                    default: pack.i++;// throw error
                }
            }
            // guard against overflow (pack.i > data.length())
            if (!depth) this->forge(this->data.substr(pincer, pack.i-pincer));
            continue;
        }

        if (p == '['){
            this->lists.createList(id);
            std::vector<std::string> values;
            while (data[pack.i] == '"'){
                pack = read(this->data, pack);
                values.push_back(pack.id);
            }

            if (data[pack.i] != ']') {
                std::cout << "objectify list formation irregularity: " << p << std::endl;
            } else {
                this->lists.set(id, values);
            }
            
            pack.i++;
        }
    }

    return this;
}

std::string node::refresh(){
    auto encapsulate = [](std::string identifier){
        return "\"" + identifier + "\"";
    };

    this->data = encapsulate(this->name) + "{";

    for (auto &&ID_pair : this->attributes.getList()){
        this->data = this->data + encapsulate(ID_pair[0]) + "=" + encapsulate(ID_pair[1]);
    }

    for (auto &&list__ : this->lists.getLists()){
        this->data = this->data + encapsulate(list__.getName()) + '[';
        for (auto &&value : list__.getValues()){
            this->data = this->data + encapsulate(value);
        }
        this->data = this->data + ']';
    }

    for (auto &child : this->children){
        // directly update data// test no '&'
        this->data = this->data + child.refresh();
    }

    this->data = this->data + "}";

    return this->data;
}

std::string node::getName(){
    return this->name;
}

std::vector<node> node::getChildren(){
    return this->children;
}

node* node::deleteChild(std::string childIdentifier){
    for (std::size_t i{this->children.size()-1};  i != SIZE_MAX; i--){
        if (this->children[i].getName() == childIdentifier) this->children.erase(this->children.begin() + i);
    }
    return this;
}

node* node::forge(node child){
    this->deleteChild(child.getName());
    this->children.push_back(child);
    return this;
}

node* node::forge(std::string childData){
    return this->forge(node{childData});
}

node* node::forge(std::vector<node> children){
    for (auto &&child : children){
        this->forge(child);
    }
    return this;
}

node node::get(std::string childIdentifier){
    for (auto &&child : this->children){
        if (child.getName() == childIdentifier){
            return child;
        }
    }
    return node{"\"" + childIdentifier + "\"{}"};
}

bool node::check(std::string childIdentifier){
    for (auto &&child : this->children){
        if (child.getName() ==  childIdentifier) return true;
    }
    return false;
}

#endif