#include "CollisionSystem.hpp"

Rect CollisionSystem::buildPlayerRect(const Player& player) {
    return Rect(player.get_X(), player.get_Y(),
                player.get_largura(), player.get_altura());
}

bool CollisionSystem::verificarColisao(Player& player,
                                       std::vector<ColetavelData>& coletaveis,
                                       Stats& stats,
                                       bool& flashAtivado) {
    Rect hitboxGisele = buildPlayerRect(player);
    bool bateuAlgo = false;

    for (auto& c : coletaveis) {
        // se ja foi consumido nesse frame, pula
        if (c.rect.getX() <= CONSUMIDO_X) continue;

        if (hitboxGisele.colliderect(c.rect)) {
            // 0=banana, 1=camera, 2=rosa, mesma convencao do jogo original
            switch (c.sprite_index) {
                case 0:
                    Banana::efeito_banana(stats);
                    break;
                case 1:
                    Camera::efeito_camera(stats);
                    flashAtivado = true;
                    break;
                case 2:
                    Rosa::efeito_rosa(stats);
                    break;
            }

            // marca como consumido, o Spawner reposiciona depois
            c.rect.setX(CONSUMIDO_X);
            bateuAlgo = true;
        }
    }

    return bateuAlgo;
}
