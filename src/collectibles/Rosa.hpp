#ifndef ROSA_HPP
#define ROSA_HPP
#include "Collectible.hpp"
#include <map>
#include <string>

class Rosa : public Base {
public:
    static void efeito_rosa(std::map<std::string, int>& contadores) {
        contadores["rosa"] += 1;
    }
};

#endif