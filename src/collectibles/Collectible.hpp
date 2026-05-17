#ifndef COLLECTIBLE_HPP
#define COLLECTIBLE_HPP

#include <vector>
#include <string>
#include <map>
#include <random>

struct Rect {
    float x, y, w, h;
};

struct ColetavelData {
    Rect rect;
    int sprite_index; 
};

class Base {
public:
    int tamanho_coletavel = 40;
    std::vector<int> alturas_coletaveis = {565, 480, 460};
    float distancia_minima_x = 250;

    Base();
    ColetavelData gerar_coletavel(int sprite_index, std::vector<int> alturas_ocupadas, std::vector<float> xs_ocupados, int largura_tela);
};

#endif