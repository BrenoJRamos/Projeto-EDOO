#ifndef BANANA_HPP
#define BANANA_HPP
#include "Collectible.hpp"
#include "../utils/Stats.hpp"

class Banana : public Base {
public:
    // efeito ao colidir com banana
    static void efeito_banana(Stats& stats) {
        stats.adicionarBanana();
    }
};

#endif
