#ifndef TERMINALRENDERER_HPP
#define TERMINALRENDERER_HPP

#include "Game.hpp"
#include <string>
#include <vector>
#include <sstream>

// tamanho do canvas ASCII que vai pro terminal
static const int CANVAS_W = 80;
static const int CANVAS_H = 20;

// herda de Game e pluga renderizacao + input via ANSI/termios
class TerminalRenderer : public Game {
private:
    // monto o frame inteiro aqui antes de soltar no stdout
    std::vector<std::string> canvas;

    // termios em raw mode? guardo pra saber se preciso restaurar
    bool raw_mode_ativo;

    // contador de frames pra alternar o sprite da Gisele
    int frame_counter;

    // se o jogador pediu replay nas telas de fim
    bool replay_pedido;

public:
    explicit TerminalRenderer(int largura = 900, int altura = 700);
    ~TerminalRenderer();

    void renderizar() override;
    void processarInput() override;

    // roda partida inteira e devolve se quer jogar de novo
    bool rodarComRenderer();

private:
    void ativarRawMode();
    void desativarRawMode();

    // devolve o char lido ou 0 se nada na fila
    char lerTeclaNaoBloqueante();

    void limparCanvas();

    // mapeia coordenada logica (0..900, 0..700) pro canvas
    void plotar(float x_logico, float y_logico, char c);
    void plotarString(float x_logico, float y_logico, const std::string& s);

    // versoes que escrevem direto na grade do canvas
    void plotarCanvas(int col, int row, char c);
    void plotarCanvasString(int col, int row, const std::string& s);

    void desenharCenario();
    void desenharGisele();
    void desenharColetaveis();
    void desenharHUD();
    void desenharFlash();

    void flushCanvas();

    void telaInicio();
    void telaVitoria();
    void telaDerrota();

    void moverCursorInicio();
    void limparTerminal();
};

#endif
