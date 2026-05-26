#include "Collectible.hpp"
#include <algorithm>
#include <cmath>

Base::Base() {}

ColetavelData Base::gerar_coletavel(const std::vector<float>& alturas_ocupadas, const std::vector<float>& xs_ocupados, int largura_tela) {
    static std::mt19937 gen(std::random_device{}()); //gerador aleatório, semeado uma única vez

    std::vector<int> alturas_disponiveis; //alturas que ainda não estão ocupadas
    for (int altura_possivel : alturas_coletaveis){
        //se a altura não está na lista das ocupadas, ela está livre
        if (std::find(alturas_ocupadas.begin(), alturas_ocupadas.end(), (float)altura_possivel) == alturas_ocupadas.end()) {
            alturas_disponiveis.push_back(altura_possivel);
        }
    }

    int altura;
    if (alturas_disponiveis.empty()){ //se todas estiveram ocupadas, sorteia qualquer altura
        std::uniform_int_distribution<> disH(0, (int)alturas_coletaveis.size() - 1);
        altura = alturas_coletaveis[disH(gen)];
    } else{ //sorteia entre as alturas livres
        std::uniform_int_distribution<> disH(0, (int)alturas_disponiveis.size() - 1);
        altura = alturas_disponiveis[disH(gen)];
    }

    float pos_x = (float)(largura_tela + 200);//x inicial: fora da tela, à direita
    std::uniform_int_distribution<> disX(largura_tela + 200, largura_tela + 900);
    for (int tentativas = 0; tentativas < 11; ++tentativas) { // até 11 tentativas de achar um x espaçado
        pos_x = (float)disX(gen); // sorteia uma posição x
        bool espaco_ok = true;
        for (float outro_x : xs_ocupados) { // verifica distância para os outros
            if (std::abs(pos_x - outro_x) < distancia_minima_x) {//verifica se ta muito perto dos outros coletaveis
                espaco_ok = false;
                break;
            }
        }
        if (espaco_ok) break;//se acha um espaco bom ai para
    }

    std::uniform_int_distribution<> disTipo(0, (int)tipos_coletaveis.size() - 1); //sorteia o tipo

    ColetavelData coletavel;
    coletavel.rect = {pos_x, (float)altura, (float)tamanho_coletavel, (float)tamanho_coletavel}; //posição e tamanho
    coletavel.tipo = tipos_coletaveis[disTipo(gen)]; //banana, camera ou rosa
    return coletavel;
}