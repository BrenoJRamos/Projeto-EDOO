#include "collectible.hpp"
#include <cmath>

class Camera : public Base {
public:
    bool flash = false;
    float flash_raio = 0;
    float flash_velocidade = 25;
    float flash_raio_max = 0;
    int flash_tempo_tela_total = 0;
    int flash_duracao_tela_total = 30;

    static void efeito_camera(std::map<std::string, int>& contadores) {
        contadores["camera"] += 1;
    }

    void iniciar_flash(int largura_tela, int altura_tela) {
        flash = true;
        flash_raio = 0;
        flash_raio_max = std::hypot(largura_tela, altura_tela);
        flash_tempo_tela_total = 0;
    }

    void atualizar_logica_flash() {
        if (!flash) return;

        if (flash_raio < flash_raio_max) {
            flash_raio += flash_velocidade;
        } else {
            flash_tempo_tela_total += 1;
            if (flash_tempo_tela_total >= flash_duracao_tela_total) {
                flash = false;
            }
        }
    }
};