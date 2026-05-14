#include <iostream>
#include <string>
#include <ctime>
#include <vector>
#include <map>
using namespace std;

// # inicializando pygame + criando tela + inicializando variáveis globais + aceleração gradual do cenário
// altura_tela = 700
// largura_tela = 900
// dimensoes_tela = (largura_tela, altura_tela)
// pygame.init()    
// pygame.display.set_caption('Gisele Bundchen VS As Forças Do Mal')
// tela = pygame.display.set_mode(dimensoes_tela)

// DAR UMA OLHADA DEPOIS
// relogio = pygame.time.Clock()
// ACELERAR = pygame.USEREVENT + 1
// pygame.time.set_timer(ACELERAR, 10000)

// # importando classes
#include "Player.hpp"
#include "Collectible.hpp"
#include "CollisionSystem.hpp"
#include "Spawner.hpp"
// from cenarios.tela_inicial import TelaInicial
// from cenarios.tela_final import TelaFinal
// from cenarios.desfile import Desfile
// from personagens.gisele import Gisele
// from coletaveis.base import Base
// from coletaveis.banana import Banana
// from coletaveis.camera import Camera
// from coletaveis.rosa import Rosa

//temporario
struct Coletavel{
    int x, y;
    string nome;
};
bool run = true;

int main(){
 
    run = true;
    bool finalizando = false;
    bool perdeu = false;
    // som_vitoria_played = false
    // som_derrota_played = false

    //dependo dos outros arquivos
    gisele.player_y = 500; // ou a posição inicial dela
    gisele.player_x = 100;
    bool gisele.esta_pulando = false;
    bool transicao_final = false;

    //PROVAVELMENTE NAO VOU USAR
    // # configurando cenário
    // tela_inicial.rodar()
    // bg_image, bg_width, tiles, scroll = Desfile.iniciar_passarela(dimensoes_tela, largura_tela)

    //definindo distância
    int distancia_pixels = 0;
    int pixels_por_metro = 20;
    int meta_metros = 700;
    int vel_coletavel = 7;

    //dicionarios com os contadores
    // contadores = {
    //     "banana": 0,   
    //     "camera": 0,   
    //     "rosa": 0    
    // }

    map<string, int> contadores = {{"banana", 0}, {"camera", 0}, {"rosa", 0}};

    vector<Coletavel> coletaveis;

    for (int i = 0; i <3;i++){
        vector<int> alturas_ocupadas;
        vector<int> xs_ocupados;
        for (Coletavel c: coletaveis){
            alturas_ocupadas.push_back(c.y);
            xs_ocupados.push_back(c.x);
        }
        coletaveis.push_back(gerar_coletavel(alturas_ocupadas, xs_ocupados, largura_tela)); //ajustar depois
    }

    // loop principal
    while (run == true){
        //variaveis locais
        delta_time = relogio.tick(30) / 1000.0 // tempo decorrido em segundos
        meta_metros = 500
        tela.fill((0,0,0))

        for event in pygame.event.get(): 
            if event.type == QUIT: # fechar a janela do jogo
                pygame.quit()
                exit()

            if event.type == KEYDOWN: # identificar o w ou a seta do pulo ou espaço
                if event.key == K_w or event.key == K_UP or event.key == K_SPACE:
                    gisele.pular()

            if event.type == ACELERAR and vel_coletavel < 20:
                vel_coletavel += 1

        # chamando as funções de gisele
        gisele.atualizar_fisica()
        gisele.atualizar_animacao(delta_time)
        
        # incrementando a distância percorrida
        distancia_pixels += 7
        distancia_metros = distancia_pixels // pixels_por_metro

        if not finalizando:
            for i in range(tiles):
                tela.blit(bg_image, (i * bg_width + scroll, 0))
            scroll -= (7 + vel_coletavel)
            if abs(scroll) > bg_width:
                scroll = 0
        if meta_metros <= distancia_metros-50: # checando finalizando de novo 
            finalizando = True
        if finalizando:
            # estado de finalização: o cenário para de rodar em loop
            # e desenhamos a passarela de chegada deslizando uma única vez
            tela.blit(LINHA_CHEGADA, (0, 0))
            
            gisele.player_x += 5

            if gisele.player_x >= largura_tela:
                 run = false # fim do jogo / tela de vitória
                 transicao_final = True


        if abs(scroll) > bg_width:
            scroll = 0

        # movimentação dos coletáveis
        if not finalizando:
            for coletavel in coletaveis:
                coletavel["rect"].x -= vel_coletavel

                if coletavel["rect"].right < 0:
                    alturas_ocupadas = [c["rect"].y for c in coletaveis if c != coletavel]
                    xs_ocupados = [c["rect"].x for c in coletaveis if c != coletavel]
                    coletavel.update(
                        base_engine.gerar_coletavel(
                            random.choice(base_engine.sprites_coletaveis),
                            alturas_ocupadas,
                            xs_ocupados,
                            largura_tela
                        )
                    )

            gisele_rect = gisele.get_rect()

        //colisões
        for coletavel in coletaveis:
            if gisele_rect.colliderect(coletavel["rect"]):
                if coletavel["coletavel"] == base_engine.sprites_coletaveis[2]:
                    Rosa.efeito_rosa(contadores)
                else:
                    if coletavel["coletavel"] == base_engine.sprites_coletaveis[0]:
                        Banana.efeito_banana(contadores)
                    elif coletavel["coletavel"] == base_engine.sprites_coletaveis[1]:
                        Camera.efeito_camera(contadores)
                        camera.iniciar_flash(largura_tela, altura_tela)

                alturas_ocupadas = [c["rect"].y for c in coletaveis if c != coletavel]
                xs_ocupados = [c["rect"].x for c in coletaveis if c != coletavel]
                coletavel.update(
                    base_engine.gerar_coletavel(
                        random.choice(base_engine.sprites_coletaveis),
                        alturas_ocupadas,
                        xs_ocupados,
                        largura_tela
                    )
                )

        //OLHAR DEPOIS
        //mostrando os contadores na tela
        for coletavel in coletaveis:
            tela.blit(coletavel["coletavel"], (coletavel["rect"]))

        // # mostrando gisele na tela
        // gisele.desenhar(tela)

        // # HUD 
        // fundo_x = 20
        // fundo_y = 8

        tela.blit(HUD_BG, (fundo_x, fundo_y))

        margin_x = fundo_x + 20

        texto_distancia = FONTE_HUD.render(f"{distancia_metros} m", True, (0, 0, 0))
        tela.blit(texto_distancia, (margin_x, fundo_y + 55))

        icones = ["banana", "camera", "rosa"]
        for i, sprite in enumerate(base_engine.sprites_coletaveis):
            tela.blit(sprite, (margin_x, 25))
            nome_item = icones[i]
            texto = FONTE_HUD.render(str(contadores[nome_item]), True, (0, 0, 0))
            tela.blit(texto, (margin_x + 45, 30))
            margin_x += 65

        //flash do paparazzi
        camera.desenhar_flash(tela, dimensoes_tela)

        # fim do jogo
        if contadores["banana"] >= 3 or contadores["camera"] >= 3:
            run = false
            perdeu = True
            # tela de game over
        elif distancia_metros >= meta_metros-50:
            finalizando = True
        pygame.display.update()

        if perdeu:
            # Chame o método da tela final que exibe derrota
            tela_final = TelaFinal(tela, contadores, distancia_metros, meta_metros)
            return tela_final.run() // retornar True para reiniciar ou false para sair
                
        if finalizando:
        if transicao_final:
            tela_final = TelaFinal(tela, contadores, distancia_metros, meta_metros)
            return tela_final.run()

# chamando função principal
if __name__ == "__main__":
    while True: # Loop de reinicialização do jogo
        jogar_novamente = main()
        if not jogar_novamente:
            break
    pygame.quit()