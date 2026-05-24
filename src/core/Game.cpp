#include "Game.hpp"

Game::Game(int largura, int altura)
    : largura_tela(largura),
      altura_tela(altura),
      spawner(largura),
      stats(500),
      estado(GameState::RODANDO),
      flash_ativado(false),
      delta_time(1.0f / FPS)
{
    gisele.setPosition(100.0f, 525.0f);
    spawner.inicializar(coletaveis);
}

void Game::processarInput() {
    // input capturado pelo TerminalRenderer via leitura nao-bloqueante
    // esse metodo existe pra fechar o ciclo update/render do loop
}

void Game::atualizar() {
    if (estado != GameState::RODANDO && estado != GameState::FINALIZANDO)
        return;

    // fisica da Gisele
    gisele.update_fisica(delta_time);

    // distancia anda sempre, mesmo no FINALIZANDO, pra cena de saida fluir
    stats.avancarDistancia(PIXELS_POR_FRAME, PIXELS_POR_METRO);

    if (estado == GameState::RODANDO) {
        spawner.atualizar(coletaveis);
        colisao.verificarColisao(gisele, coletaveis, stats, flash_ativado);
        camera_obj.atualizar_logica_flash();
    }

    if (estado == GameState::FINALIZANDO) {
        // cena final: gisele corre pra fora da tela
        gisele.setX(gisele.get_X() + 5);
    }
}

void Game::checarEstado() {
    if (estado == GameState::RODANDO) {
        if (stats.perdeu()) {
            estado = GameState::DERROTA;
        } else if (stats.venceu()) {
            estado = GameState::FINALIZANDO;
        }
    }

    if (estado == GameState::FINALIZANDO) {
        if (gisele.get_X() >= largura_tela) {
            estado = GameState::VITORIA;
        }
    }
}

bool Game::rodar() {
    while (estado == GameState::RODANDO ||
           estado == GameState::FINALIZANDO) {
        processarInput();
        atualizar();
        checarEstado();
        renderizar();  // no-op aqui, TerminalRenderer sobrescreve
    }

    // decisao de replay fica no TerminalRenderer (Prompt 5)
    return false;
}
