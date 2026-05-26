#ifndef ROSA_HPP
#define ROSA_HPP
#include "Collectible.hpp"
#include <map>
#include <string>
#include <iostream>

class Rosa : public Base {
public:
    void efeito(std::map<std::string, int>& contadores) override{
        contadores["rosa"] += 1;
    }

    ~Rosa(){
        std::cout << "objeto Rosa destruido\n";
    }
};

#endif