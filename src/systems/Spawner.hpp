#ifndef SPAWNER_HPP
#define SPAWNER_HPP

#include <vector>
#include "../collectibles/Collectible.hpp"
#include "../utils/Rect.hpp"

// gerencia o pool de coletaveis: spawn inicial, scroll pra esquerda e
// reposicao quando saem da tela ou sao consumidos pela colisao
class Spawner {
private:
    Base base_engine;
    int largura_tela;
    int vel_coletavel;
    int frames_desde_ultimo_aumento;

    // 300 frames a 30 FPS = 10s, bate com o pygame.time.set_timer do original
    static const int FRAMES_POR_AUMENTO = 300;
    static const int VEL_MAX = 20;
    static const int NUM_COLETAVEIS = 3;

public:
    explicit Spawner(int largura_tela);

    void inicializar(std::vector<ColetavelData>& coletaveis);
    void atualizar(std::vector<ColetavelData>& coletaveis);

    int getVelColetavel() const { return vel_coletavel; }
};

#endif
