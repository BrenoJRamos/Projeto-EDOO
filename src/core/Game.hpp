#ifndef GAME_HPP
#define GAME_HPP

#include "../player/Player.hpp"
#include "../collectibles/Collectible.hpp"
#include "../collectibles/Camera.hpp"
#include "../systems/CollisionSystem.hpp"
#include "../systems/Spawner.hpp"
#include "../utils/Stats.hpp"
#include "GameState.hpp"
#include <vector>

// integrador central. dona dos subsistemas e roda o loop principal.
// renderizar() e virtual de proposito, o TerminalRenderer herda e
// pluga a UI sem mexer na logica daqui
class Game {
private:
    // dimensoes da "tela", uso so pra contas de posicao, nao tem GUI
    int largura_tela;
    int altura_tela;

    // entidades
    Player gisele;
    Camera camera_obj;

    // sistemas
    CollisionSystem colisao;
    Spawner spawner;

    // estado e contadores
    Stats stats;
    GameState estado;
    std::vector<ColetavelData> coletaveis;

    // controle de loop
    bool flash_ativado;
    float delta_time;
    static const int FPS = 30;
    static const int PIXELS_POR_METRO = 20;
    static const int PIXELS_POR_FRAME = 7;

public:
    explicit Game(int largura = 900, int altura = 700);

    // roda uma partida inteira, retorna true se o jogador quiser de novo
    bool rodar();

private:
    void processarInput();
    void atualizar();
    void checarEstado();

    // hook de render: no-op aqui, TerminalRenderer (Prompt 5) sobrescreve
    virtual void renderizar() {}
};

#endif
