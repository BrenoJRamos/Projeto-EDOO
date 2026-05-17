#include "collectible.hpp"
class Banana : public Base {
public:
    static void efeito_banana(std::map<std::string, int>& contadores) {
        contadores["rosa"] = 0;      
        contadores["banana"] += 1;   
    }
};