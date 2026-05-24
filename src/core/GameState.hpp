#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

// estados possiveis do jogo em qualquer momento
enum class GameState {
    RODANDO,      // loop principal ativo
    FINALIZANDO,  // gisele chegou perto da meta, modo de saida
    VITORIA,      // gisele cruzou a linha de chegada
    DERROTA       // banana >= 3 ou camera >= 3
};

#endif
