#include "Collectible.hpp"
#include <algorithm>

Base::Base() {}

ColetavelData Base::gerar_coletavel(int sprite_index, std::vector<int> alturas_ocupadas, std::vector<float> xs_ocupados, int largura_tela) {
    std::random_device rd;
    std::mt19937 gen(rd());

    std::vector<int> alturas_disponiveis;
    for (int h : alturas_coletaveis) {
        if (std::find(alturas_ocupadas.begin(), alturas_ocupadas.end(), h) == alturas_ocupadas.end()) {
            alturas_disponiveis.push_back(h);
        }
    }

    int altura;
    if (alturas_disponiveis.empty()) {
        std::uniform_int_distribution<> disH(0, alturas_coletaveis.size() - 1);
        altura = alturas_coletaveis[disH(gen)];
    } else {
        std::uniform_int_distribution<> disH(0, alturas_disponiveis.size() - 1);
        altura = alturas_disponiveis[disH(gen)];
    }

    float x;
    std::uniform_int_distribution<> disX(largura_tela + 200, largura_tela + 900);
    
    for (int tentativas = 0; tentativas < 11; ++tentativas) {
        x = disX(gen);
        bool espaco_ok = true;
        for (float outro_x : xs_ocupados) {
            if (std::abs(x - outro_x) < distancia_minima_x) {
                espaco_ok = false;
                break;
            }
        }
        if (espaco_ok) break;
    }

    return {{x, (float)altura, (float)tamanho_coletavel, (float)tamanho_coletavel}, sprite_index};
}