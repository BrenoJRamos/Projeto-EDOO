#ifndef BANANA_HPP
#define BANANA_HPP
#include "Collectible.hpp"
#include <map>
#include <string>

class Banana : public Base {
public:
    static void efeito_banana(std::map<std::string, int>& contadores) {
        contadores["rosa"] = 0;      
        contadores["banana"] += 1;   
    }
};

#endif