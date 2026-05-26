#ifndef BANANA_HPP
#define BANANA_HPP
#include "Collectible.hpp"
#include <map>
#include <string>
#include <iostream>

class Banana : public Base {
public:
    void efeito(std::map<std::string, int>& contadores) override{
        contadores["rosa"] = 0;
        contadores["banana"] += 1;
    }

    ~Banana(){
        std::cout << "objeto Banana destruido\n";
    }
};

#endif