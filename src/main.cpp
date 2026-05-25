#include <Game.hpp>

int main() {
    Game game;
    while (true) {
        bool jogar_novamente = game.jogo();
        if (jogar_novamente == false) {
            break;
        }
    }
    return 0;
}