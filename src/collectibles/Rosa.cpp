#include "collectible.hpp"

class Rosa : public Base {
public:
    static void efeito_rosa(std::map<std::string, int>& contadores) {
        contadores["rosa"] += 1;
    }
};