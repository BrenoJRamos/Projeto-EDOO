#include "Spawner.hpp"
#include <cstdlib>

Spawner::Spawner(int largura_tela)
    : largura_tela(largura_tela),
      vel_coletavel(7),
      frames_desde_ultimo_aumento(0) {}

void Spawner::inicializar(std::vector<ColetavelData>& coletaveis) {
    for (int i = 0; i < NUM_COLETAVEIS; ++i) {
        // monta a lista do que ja ta na tela pra gerar_coletavel evitar overlap
        std::vector<int> alturas_ocupadas;
        std::vector<float> xs_ocupados;
        for (const auto& c : coletaveis) {
            alturas_ocupadas.push_back((int)c.rect.getY());
            xs_ocupados.push_back(c.rect.getX());
        }

        int sprite = std::rand() % 3;
        coletaveis.push_back(
            base_engine.gerar_coletavel(sprite, alturas_ocupadas, xs_ocupados, largura_tela)
        );
    }
}

void Spawner::atualizar(std::vector<ColetavelData>& coletaveis) {
    // 1) scroll pra esquerda
    for (auto& c : coletaveis) {
        c.rect.setX(c.rect.getX() - vel_coletavel);
    }

    // 2) repor o que saiu da tela ou foi consumido na colisao
    for (size_t i = 0; i < coletaveis.size(); ++i) {
        ColetavelData& alvo = coletaveis[i];
        bool foraDaTela = (alvo.rect.getX() + alvo.rect.getW() < 0);
        bool consumido = (alvo.rect.getX() <= -9999.0f);

        if (foraDaTela || consumido) {
            std::vector<int> alturas_ocupadas;
            std::vector<float> xs_ocupados;
            for (size_t j = 0; j < coletaveis.size(); ++j) {
                if (j == i) continue;
                alturas_ocupadas.push_back((int)coletaveis[j].rect.getY());
                xs_ocupados.push_back(coletaveis[j].rect.getX());
            }

            int sprite = std::rand() % 3;
            coletaveis[i] = base_engine.gerar_coletavel(sprite, alturas_ocupadas, xs_ocupados, largura_tela);
        }
    }

    // 3) aumenta a velocidade de tempos em tempos, sobe ate o teto e para
    frames_desde_ultimo_aumento++;
    if (frames_desde_ultimo_aumento >= FRAMES_POR_AUMENTO && vel_coletavel < VEL_MAX) {
        vel_coletavel++;
        frames_desde_ultimo_aumento = 0;
    }
}
