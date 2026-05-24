#include "core/TerminalRenderer.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>

// ponto de entrada: mostra a arquitetura pro professor e dispara o jogo
int main() {
    std::cout << "\n=== Gisele Bundchen vs As Forcas do Mal (C++) ===\n";
    std::cout << "Arquitetura:\n";
    std::cout << "  utils/        : Rect, Vector2, Stats\n";
    std::cout << "  player/       : Player\n";
    std::cout << "  collectibles/ : Base, Banana, Camera, Rosa\n";
    std::cout << "  systems/      : CollisionSystem, Spawner\n";
    std::cout << "  core/         : GameState, Game, TerminalRenderer\n\n";

    std::srand((unsigned)std::time(nullptr));

    bool jogar_novamente = true;
    while (jogar_novamente) {
        TerminalRenderer jogo;
        jogar_novamente = jogo.rodarComRenderer();
    }

    std::cout << "\nObrigado por jogar!\n";
    return 0;
}
