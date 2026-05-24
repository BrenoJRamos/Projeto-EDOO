#ifndef COLLISIONSYSTEM_HPP
#define COLLISIONSYSTEM_HPP

#include <vector>
#include "../utils/Rect.hpp"
#include "../utils/Stats.hpp"
#include "../player/Player.hpp"
#include "../collectibles/Collectible.hpp"
#include "../collectibles/Banana.hpp"
#include "../collectibles/Camera.hpp"
#include "../collectibles/Rosa.hpp"

// sistema de colisao. tudo encapsulado aqui pra Game so chamar
// verificarColisao() e seguir a vida
class CollisionSystem {
private:
    // monta o Rect da Gisele toda vez que vai testar, mais simples do que
    // ficar guardando estado de hitbox em outro lugar
    static Rect buildPlayerRect(const Player& player);

public:
    // sentinela: coletavel marcado com esse x ja foi consumido nesse frame,
    // o Spawner cuida de reposicionar dps
    static constexpr float CONSUMIDO_X = -9999.0f;

    // retorna true se bateu em pelo menos um coletavel nesse frame
    bool verificarColisao(Player& player,
                          std::vector<ColetavelData>& coletaveis,
                          Stats& stats,
                          bool& flashAtivado);
};

#endif
