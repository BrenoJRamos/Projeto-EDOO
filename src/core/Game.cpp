#include <Collectible.hpp>
#include <Banana.hpp>
#include <Camera.hpp>
#include <Rosa.hpp>
#include <Player.hpp>
#include <CollisionSystem.hpp>
#include <Spawner.hpp>
#include <iostream>
#include <vector>
#include <map>
using namespace std;


//inicializando pygame + criando tela + inicializando variáveis globais + aceleração gradual do cenário
int altura_tela = 700;
int largura_tela = 900;
//dimensoes_tela = (largura_tela, altura_tela)
//pygame.init()    
//pygame.display.set_caption('Gisele Bundchen VS As Forças Do Mal')
// tela = pygame.display.set_mode(dimensoes_tela)
// relogio = pygame.time.Clock()
// ACELERAR = pygame.USEREVENT + 1
// pygame.time.set_timer(ACELERAR, 10000)


// # instanciando objetos para acessar a classe
// base_engine = Base()
// gisele = Gisele()
// banana = Banana()
// camera = Camera()
// tela_inicial = TelaInicial(tela)

Player gisele;
Banana banana;
Base base_engine;
Camera camera;




bool jogo(){

    //global run 
    bool run = true;;
    bool finalizando = false;
    bool perdeu = false;
    bool som_vitoria_played = false;
    bool som_derrota_played = false;
    gisele.setPosition(100, 500); //= 500 # ou a posição inicial dela
    //gisele.player_x = 100
    //gisele.esta_pulando = false;
    bool transicao_final = false;

    // # configurando cenário
    // tela_inicial.rodar()
    // bg_image, bg_width, tiles, scroll = Desfile.iniciar_passarela(dimensoes_tela, largura_tela)

    //definindo distância
    int distancia_pixels = 0;
    int pixels_por_metro = 20;
    int meta_metros = 700;
    int vel_coletavel = 7;

    // # dicionários com os contadores
    // contadores = {
    //     "banana": 0,   
    //     "camera": 0,   
    //     "rosa": 0    
    // }

    map<string, int> contadores = {{"Banana", 0}, {"Rosa", 0}, {"Moeda", 0}};


    vector<ColetavelData> coletaveis;

    for (int i = 0; i <3; i++){
        vector<int> alturas_ocupadas;// = [c["rect"].y for c in coletaveis]
        vector<int> xs_ocupados;// = [c["rect"].x for c in coletaveis]

        for (auto &c: coletaveis){
            alturas_ocupadas.push_back(c.rect.y);
            xs_ocupados.push_back(c.rect.x);
        }

        //######################################//######################################
        //######################################//######################################
        //######################################//######################################
        //sprite_coletavel = random.choice(base_engine.sprites_coletaveis)
        coletaveis.push_back(base_engine.gerar_coletavel(alturas_ocupadas, xs_ocupados, largura_tela)); //MODIFICAR
        //     base_engine.gerar_coletavel(
        //         sprite_coletavel,
        //         alturas_ocupadas,
        //         xs_ocupados,
        //         largura_tela
        //     )
        // )
        //######################################//######################################
        //######################################//######################################
        //######################################//######################################

    }

    //# loop principal
    while (run == true;){
        //# variaveis locais
        float delta_time = 1.0/30;//relogio.tick(30) / 1000.0 # tempo decorrido em segundos
        int meta_metros = 500;
        //tela.fill((0,0,0))


//VERIFICAR VERIFICAR VERIFICAR VERIFICAR VERIFICAR VERIFICAR VERIFICAR VERIFICAR VERIFICAR VERIFICAR VERIFICAR
        // for event in pygame.event.get(): 
        //     if event.type == QUIT: # fechar a janela do jogo
        //         pygame.quit()
        //         exit()

        //     if event.type == KEYDOWN: # identificar o w ou a seta do pulo ou espaço
        //         if event.key == K_w or event.key == K_UP or event.key == K_SPACE:
        //             gisele.pular()

        //     if event.type == ACELERAR and vel_coletavel < 20:
        //         vel_coletavel += 1

        //# chamando as funções de gisele
        gisele.update_fisica(delta_time);
        //gisele.atualizar_animacao(delta_time);
        
        //# incrementando a distância percorrida
        distancia_pixels += 7;
        int distancia_metros = distancia_pixels / pixels_por_metro;

        if (meta_metros <= distancia_metros - 50) {
            finalizando = true;;
        }

        if (finalizando == true;){
            // for i in range(tiles):
            //     tela.blit(bg_image, (i * bg_width + scroll, 0))
            // scroll -= (7 + vel_coletavel)
            // if abs(scroll) > bg_width:
            //     scroll = 0
            gisele.setX(gisele.get_X() + 5);
            if (gisele.get_X() >= largura_tela){
                 run = false; //# fim do jogo / tela de vitória
                 transicao_final = true;;
            }
        }
            
        // if finalizando:
        //     # estado de finalização: o cenário para de rodar em loop
        //     # e desenhamos a passarela de chegada deslizando uma única vez
        //     tela.blit(LINHA_CHEGADA, (0, 0))
            
        //     gisele.player_x += 5

        //     if gisele.player_x >= largura_tela:
        //          run = false; # fim do jogo / tela de vitória
        //          transicao_final = true;


        // if abs(scroll) > bg_width:
        //     scroll = 0

        # movimentação dos coletáveis
        if (finalizando == false){
            for (auto& coletavel : coletaveis){
                coletavel.rect.x -= vel_coletavel;

                if (coletavel.rect.x + coletavel.rect.w < 0){
                    vector<int> alturas_ocupadas; //= [c["rect"].y for c in coletaveis if c != coletavel]
                    vector<float> xs_ocupados; //= [c["rect"].x for c in coletaveis if c != coletavel]
                    
                    for (auto &c : coletaveis){
                        if (&c != &coletavel){
                            alturas_ocupadas.push_back(c.rect.y);
                            xs_ocupados.push_back(c.rect.x);
                        }
                    }
                    coletavel = base_engine.gerar_coletavel(alturas_ocupadas, xs_ocupados, largura_tela); //MODIFICAR
                //se usasse push_back, adiconaria um novo coletavel quando um coletavel saisse da tela
                }
            }
        }
            Rect gisele_rect = gisele.get_rect(); //adicionar

        //FALTAM AS COLISOES
        //# colisões
        for (auto &coletavel :coletaveis){
            if (gisele_rect.colliderect(coletavel["rect"]):
                if coletavel["coletavel"] == base_engine.sprites_coletaveis[2]:
                    Rosa.efeito_rosa(contadores)
                    som_rosa.play()
                else:
                    if coletavel["coletavel"] == base_engine.sprites_coletaveis[0]:
                        Banana.efeito_banana(contadores)
                        som_banana.play()
                    elif coletavel["coletavel"] == base_engine.sprites_coletaveis[1]:
                        Camera.efeito_camera(contadores)
                        camera.iniciar_flash(largura_tela, altura_tela)
                        som_camera.play()

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
            }

        // # mostrando os contadores na tela
        // for coletavel in coletaveis:
        //     tela.blit(coletavel["coletavel"], (coletavel["rect"]))

        // # mostrando gisele na tela
        // gisele.desenhar(tela)

        // # HUD 
        // fundo_x = 20
        // fundo_y = 8

        // tela.blit(HUD_BG, (fundo_x, fundo_y))

        // margin_x = fundo_x + 20

        // texto_distancia = FONTE_HUD.render(f"{distancia_metros} m", true;, (0, 0, 0))
        // tela.blit(texto_distancia, (margin_x, fundo_y + 55))

        // icones = ["banana", "camera", "rosa"]
        // for i, sprite in enumerate(base_engine.sprites_coletaveis):
        //     tela.blit(sprite, (margin_x, 25))
        //     nome_item = icones[i]
        //     texto = FONTE_HUD.render(str(contadores[nome_item]), true;, (0, 0, 0))
        //     tela.blit(texto, (margin_x + 45, 30))
        //     margin_x += 65

        // # flash do paparazzi
        // camera.desenhar_flash(tela, dimensoes_tela)

        //# fim do jogo
        if (contadores["banana"] >= 3 || contadores["camera"] >= 3){
            run = false;
            perdeu = true;
            //# tela de game over
        }else if (distancia_metros >= meta_metros-50){
            finalizando = true;
        }
        //pygame.display.update()

        if (perdeu == true){
            return false;
        }
        
        if (transicao_final){
            return false;
        }

    return false;
    }
}

int main(){
    srand(time(nullptr));
    while (true){
        bool jogar_novamente = jogo();
        if (jogar_novamente == false){
            break;
        }
    }
    return 0;
}