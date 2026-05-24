#include "TerminalRenderer.hpp"
#include <iostream>
#include <sstream>
#include <string>
#include <thread>
#include <chrono>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

// ANSI: cores e movimento de cursor. funciona em WSL, Linux e Win10+
static const std::string RESET        = "\033[0m";
static const std::string BOLD         = "\033[1m";
static const std::string GREEN        = "\033[32m";
static const std::string YELLOW       = "\033[33m";
static const std::string CYAN         = "\033[36m";
static const std::string WHITE        = "\033[37m";
static const std::string RED          = "\033[31m";
static const std::string MAGENTA      = "\033[35m";
static const std::string BG_WHITE     = "\033[47m";

static const std::string CLEAR_SCREEN = "\033[2J\033[H";
static const std::string CURSOR_HOME  = "\033[H";
static const std::string HIDE_CURSOR  = "\033[?25l";
static const std::string SHOW_CURSOR  = "\033[?25h";

// guardo o estado do terminal pra restaurar no destrutor
static struct termios terminal_original;

TerminalRenderer::TerminalRenderer(int largura, int altura)
    : Game(largura, altura),
      canvas(CANVAS_H, std::string(CANVAS_W, ' ')),
      raw_mode_ativo(false),
      frame_counter(0),
      replay_pedido(false)
{
    limparTerminal();
    std::cout << HIDE_CURSOR;
    std::cout.flush();
    ativarRawMode();
}

TerminalRenderer::~TerminalRenderer() {
    desativarRawMode();
    std::cout << SHOW_CURSOR;
    std::cout << RESET;
    std::cout.flush();
}

void TerminalRenderer::ativarRawMode() {
    tcgetattr(STDIN_FILENO, &terminal_original);
    struct termios raw = terminal_original;
    raw.c_lflag &= ~(ICANON | ECHO);
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSANOW, &raw);
    raw_mode_ativo = true;
}

void TerminalRenderer::desativarRawMode() {
    if (raw_mode_ativo) {
        tcsetattr(STDIN_FILENO, TCSANOW, &terminal_original);
        raw_mode_ativo = false;
    }
}

char TerminalRenderer::lerTeclaNaoBloqueante() {
    char c = 0;
    if (read(STDIN_FILENO, &c, 1) == 1)
        return c;
    return 0;
}

void TerminalRenderer::processarInput() {
    char c = lerTeclaNaoBloqueante();
    if (c == ' ' || c == 'w' || c == 'W') {
        getGisele().jump();
    }
    // ESC ou Q derruba o jogo na hora
    if (c == 27 || c == 'q' || c == 'Q') {
        forcarDerrota();
    }
}

void TerminalRenderer::limparCanvas() {
    for (auto& linha : canvas)
        linha = std::string(CANVAS_W, ' ');
}

void TerminalRenderer::plotar(float x_logico, float y_logico, char c) {
    int col = (int)((x_logico / getLarguraTela()) * CANVAS_W);
    int row = (int)((y_logico / getAlturaTela()) * CANVAS_H);
    plotarCanvas(col, row, c);
}

void TerminalRenderer::plotarString(float x_logico, float y_logico, const std::string& s) {
    int col = (int)((x_logico / getLarguraTela()) * CANVAS_W);
    int row = (int)((y_logico / getAlturaTela()) * CANVAS_H);
    plotarCanvasString(col, row, s);
}

void TerminalRenderer::plotarCanvas(int col, int row, char c) {
    if (row >= 0 && row < CANVAS_H && col >= 0 && col < CANVAS_W)
        canvas[row][col] = c;
}

void TerminalRenderer::plotarCanvasString(int col, int row, const std::string& s) {
    for (int i = 0; i < (int)s.size(); i++)
        plotarCanvas(col + i, row, s[i]);
}

void TerminalRenderer::desenharCenario() {
    for (int col = 0; col < CANVAS_W; col++)
        plotarCanvas(col, 15, '=');
    for (int col = 0; col < CANVAS_W; col++)
        plotarCanvas(col, 14, '-');
}

void TerminalRenderer::desenharGisele() {
    // 2 frames alternados, troca a cada 4 ticks pra nao ficar epileptico
    bool frame_par = (frame_counter / 4) % 2 == 0;

    float gx = getGisele().get_X();
    float gy = getGisele().get_Y();

    plotar(gx, gy - 40, 'O');
    plotar(gx, gy - 20, '|');
    if (frame_par) {
        plotar(gx - 5, gy, '/');
        plotar(gx + 5, gy, '\\');
    } else {
        plotar(gx - 5, gy, '\\');
        plotar(gx + 5, gy, '/');
    }
}

void TerminalRenderer::desenharColetaveis() {
    for (const auto& c : getColetaveis()) {
        if (c.rect.getX() <= -9999.0f) continue;
        char simbolo;
        switch (c.sprite_index) {
            case 0: simbolo = 'B'; break;
            case 1: simbolo = 'C'; break;
            case 2: simbolo = 'R'; break;
            default: simbolo = '?';
        }
        plotar(c.rect.getX(), c.rect.getY(), simbolo);
    }
}

void TerminalRenderer::desenharHUD() {
    std::ostringstream hud;
    hud << "[ B:" << getStats().getBanana() << "/3"
        << " C:" << getStats().getCamera() << "/3"
        << " R:" << getStats().getRosa()
        << " | " << getStats().getDistanciaMetros()
        << "/" << getStats().getMetaMetros() << "m"
        << " | SPD:" << getSpawner().getVelColetavel()
        << " | SPACE=pulo  Q=sair ]";
    plotarCanvasString(0, 0, hud.str());
}

void TerminalRenderer::desenharFlash() {
    if (!getFlashAtivado()) return;
    plotarCanvasString(0, 1, std::string(CANVAS_W, ' '));
    plotarCanvasString(10, 1, "*** FLASH! PAPARAZZI! ***");
    setFlashAtivado(false);
    getCameraObj().atualizar_logica_flash();
}

void TerminalRenderer::flushCanvas() {
    moverCursorInicio();
    for (int row = 0; row < CANVAS_H; row++) {
        std::cout << "|" << canvas[row] << "|\n";
    }
    std::cout << "+" << std::string(CANVAS_W, '-') << "+\n";
    std::cout.flush();
}

void TerminalRenderer::moverCursorInicio() {
    std::cout << CURSOR_HOME;
}

void TerminalRenderer::limparTerminal() {
    std::cout << CLEAR_SCREEN;
    std::cout.flush();
}

void TerminalRenderer::renderizar() {
    limparCanvas();
    desenharCenario();
    desenharGisele();
    desenharColetaveis();
    desenharHUD();
    desenharFlash();
    flushCanvas();
    frame_counter++;

    // trava 30 FPS, mais que isso o terminal nao acompanha
    std::this_thread::sleep_for(std::chrono::milliseconds(33));
}

void TerminalRenderer::telaInicio() {
    limparTerminal();
    std::cout << GREEN << BOLD;
    std::cout << "\n";
    std::cout << "  +---------------------------------------------------------+\n";
    std::cout << "  |   GISELE BUNDCHEN VS AS FORCAS DO MAL                   |\n";
    std::cout << "  |   versao C++ | EDOO/CIn-UFPE                            |\n";
    std::cout << "  +---------------------------------------------------------+\n";
    std::cout << "\n";
    std::cout << RESET;
    std::cout << "  Desvie das bananas (B) e cameras (C).\n";
    std::cout << "  Colete rosas (R) pra marcar pontos.\n";
    std::cout << "  Chegue ao fim sem levar 3 hits!\n\n";
    std::cout << "  Controles: SPACE ou W = pulo  |  Q = sair\n\n";
    std::cout << "  Pressione SPACE para comecar...\n";
    std::cout.flush();

    char c = 0;
    while (c != ' ' && c != 'w' && c != 'W') {
        c = lerTeclaNaoBloqueante();
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
    limparTerminal();
}

void TerminalRenderer::telaVitoria() {
    limparTerminal();
    std::cout << GREEN << BOLD;
    std::cout << "\n  VITORIA! Gisele chegou ao fim da passarela!\n\n";
    std::cout << RESET;
    std::cout << getStats();
    std::cout << "\n\n  Jogar de novo? (S/N): ";
    std::cout.flush();
    desativarRawMode();
    char c = 0;
    std::cin >> c;
    ativarRawMode();
    replay_pedido = (c == 's' || c == 'S');
}

void TerminalRenderer::telaDerrota() {
    limparTerminal();
    std::cout << RED << BOLD;
    std::cout << "\n  GAME OVER! As forcas do mal venceram desta vez...\n\n";
    std::cout << RESET;
    std::cout << getStats();
    std::cout << "\n\n  Jogar de novo? (S/N): ";
    std::cout.flush();
    desativarRawMode();
    char c = 0;
    std::cin >> c;
    ativarRawMode();
    replay_pedido = (c == 's' || c == 'S');
}

bool TerminalRenderer::rodarComRenderer() {
    telaInicio();
    rodar();

    if (getEstado() == GameState::VITORIA)
        telaVitoria();
    else
        telaDerrota();

    return replay_pedido;
}
