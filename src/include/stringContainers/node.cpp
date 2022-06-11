#ifndef CHILDSTRING_CPP
#define CHILDSTRING_CPP

#include "stringContainers.h"
#include <string>
#include <array>
#include <vector>

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
            std::string identifier{};
            std::string::size_type i{};
            package(){};
            package(std::string identifier, std::string::size_type i){
                this->identifier = identifier;
                this->i = i;
            };
    };
    
    auto read = [](const std::string data, package pack){
        pack.identifier = "";
        for (pack.i++; pack.i < data.length(); pack.i++){
            if (data[pack.i] != '"') pack.identifier = pack.identifier + data[pack.i];
        }
        //if (pack.i == data.length()) throwError("node::read(): File Overran");
        pack.i++;
        return pack;
    };

    auto cut = [](std::string data, const std::array<std::string::size_type, 2> pincer){
        return data.substr(pincer[0], pincer[1]); //unsure if it works...
    };

    char p{};
    int depth{};
    std::array<std::string, 2> ID_pair{};
    std::array<std::string::size_type, 2> pincer{};

    package pack{read(this->data, package{})};
    this->name = pack.identifier;
    pack.i++;
    
    while (pack.i < this->data.length() - 1){
        pincer[0] = pack.i;
        pack = read(this->data, pack);
        ID_pair[0] = pack.identifier;
        p = this->data[pack.i++];
        if (p == '='){
            pack = read(this->data, pack);
            ID_pair[1] = pack.identifier;
            this->attributes.set(ID_pair);
        } else if (p == '{'){
            depth = 1;
            while (pack.i < this->data.length() && depth != 0){
                switch (this->data[pack.i]){
                    case '"':
                        pack = read(this->data, pack);
                        break;
                    case '{':
                        depth++;
                        pack.i++;
                        break;
                    case '}':
                        depth--;
                        pack.i++;
                        break;
                    case '=':
                        pack.i++;
                        break;
                    default:
                        pack.i++;
                        //throwError("verStr::objectify(): Unexpected character: " + String[i]);
                }
            }
            if (depth == 0){
                pincer[1] = pack.i;
                forge(cut(this->data, pincer));
            }
        }
    } // missing error checks

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
    for (auto &child : this->children){ // directly update data
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
    for (std::size_t i{this->children.size() - 1};  i != SIZE_MAX; i--){
        if (this->children[i].getName() == childIdentifier) this->children.erase(this->children.begin() + i);
    }
    return this;
}

node* node::forge(node child){
    deleteChild(child.getName());
    this->children.push_back(child);
    return this;
}

node* node::forge(std::string childData){
    return forge(node{childData});
}

node* node::forge(std::vector<node> children){
    for (auto &&child : this->children){
        forge(child);
    }
    return this;
}

node node::get(std::string childIdentifier){
    for (auto &&child : this->children) if (child.getName() == childIdentifier) return child;
    forge("\"" + childIdentifier + "\"");
    return this->children.back();
}

bool node::check(std::string childIdentifier){
    for (auto &&child : this->children){
        if (child.getName() ==  childIdentifier) return true;
    }
    return false;
}

#endif