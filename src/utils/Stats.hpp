#ifndef STATS_HPP
#define STATS_HPP

#include <iostream>

// contadores do jogo, substitui aquele map<string,int> que tava no Game.cpp
// original. centralizei aqui pra Game so chamar metodo sem se preocupar com
// regra de game-over ou conversao de pixel pra metro
class Stats {
private:
    int banana;
    int camera;
    int rosa;
    int distancia_pixels;
    int distancia_metros;
    int meta_metros;

public:
    Stats(int meta = 500)
        : banana(0), camera(0), rosa(0),
          distancia_pixels(0), distancia_metros(0), meta_metros(meta) {}

    // bater em banana zera a sequencia de rosas, e a penalidade do jogo original
    void adicionarBanana() {
        banana++;
        zerarRosa();
    }

    void adicionarCamera() { camera++; }
    void adicionarRosa() { rosa++; }
    void zerarRosa() { rosa = 0; }

    void avancarDistancia(int pixelsPorFrame, int pixelsPorMetro) {
        distancia_pixels += pixelsPorFrame;
        distancia_metros = distancia_pixels / pixelsPorMetro;
    }

    // 3 bananas ou 3 cameras e a Gisele ja era
    bool perdeu() const { return banana >= 3 || camera >= 3; }

    // margem de 50m pra considerar que chegou na meta, igual o original
    bool venceu() const { return distancia_metros >= meta_metros - 50; }

    int getBanana() const { return banana; }
    int getCamera() const { return camera; }
    int getRosa() const { return rosa; }
    int getDistanciaMetros() const { return distancia_metros; }
    int getMetaMetros() const { return meta_metros; }

    // HUD em ASCII puro, nada de emoji pra nao quebrar no cmd do Windows
    friend std::ostream& operator<<(std::ostream& os, const Stats& s) {
        os << "[ B:" << s.banana << "/3"
           << " | C:" << s.camera << "/3"
           << " | R:" << s.rosa
           << " | dist: " << s.distancia_metros << "/" << s.meta_metros << "m ]";
        return os;
    }
};

#endif
