#ifndef COLLECTIBLE_HPP
#define COLLECTIBLE_HPP

#include <vector>
#include <string>
#include <map>
#include <random>

struct Rect {
    float x;    
    float y;    
    float largura;  
    float altura;   

    //colisão AABB
    //bordas que apenas se tocam não colidem
    bool colliderect(const Rect& outro) const {
        return x < outro.x + outro.largura && x + largura > outro.x &&
               y < outro.y + outro.altura && y + altura > outro.y;
    }
};

struct ColetavelData{
    Rect rect;
    std::string tipo = "banana"; //"banana", "camera" ou "rosa"
};

class Base{
public:
    int tamanho_coletavel = 40;
    std::vector<int> alturas_coletaveis = {565, 480, 460};
    std::vector<std::string> tipos_coletaveis = {"banana", "camera", "rosa"};
    float distancia_minima_x = 250;

    Base();

    //gera um novo coletavel sem sobrepor os que ja estao na tela.
    ColetavelData gerar_coletavel(const std::vector<float>& alturas_ocupadas, const std::vector<float>& xs_ocupados, int largura_tela);
};

#endif