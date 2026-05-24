#ifndef ROSA_HPP
#define ROSA_HPP
#include "Collectible.hpp"
#include "../utils/Stats.hpp"

class Rosa : public Base {
public:
    // efeito ao colidir com rosa
    static void efeito_rosa(Stats& stats) {
        stats.adicionarRosa();
    }
};

#endif
