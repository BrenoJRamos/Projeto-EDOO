#ifndef GAME_HPP
#define GAME_HPP

#include <Collectible.hpp>
#include <Player.hpp>


class Game {
public:
    Game();

    bool jogo(); //roda uma partida e retorna true se for para jogar de novo

private:
    int altura_tela; 
    int largura_tela; 

    Player gisele;
    Base base_engine;
};

#endif