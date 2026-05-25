#include <Game.hpp>
#include <Banana.hpp>
#include <Camera.hpp>
#include <Rosa.hpp>
#include <iostream>
#include <vector>
#include <map>
#include <string>
using namespace std;

//construtor pro tamanhao da tela
Game::Game(){
    altura_tela = 700;
    largura_tela = 900;
}

bool Game::jogo(){

    bool run = true; //jogo rodando
    bool finalizando = false; //chegou perto do fim (no jogo original o cenario para de rolar)
    bool perdeu = false;
    bool transicao_final = false;//venceu

    gisele.setPosition(100, 500); //posição inicial da gisele

    //uso do getters pra mostrar os atributos
    cout << "Gisele Bundchen VS As Forcas do Mal\n";
    cout << "atributos da Gisele:\n";
    cout << "posicao inicial: x=" << gisele.get_X() << " y=" << gisele.get_Y() << "\n";
    cout << "hitbox: largura=" << gisele.get_largura() << " altura=" << gisele.get_altura() << "\n";
    cout << "velocidade horizontal: " << gisele.get_velocidadeX() << "\n";
    cout << "andando: " << (gisele.get_esta_andando() ? "sim" : "nao") << "\n\n";

    // distancia percorrida
    int distancia_pixels = 0;
    int pixels_por_metro = 20;
    int meta_metros = 500;
    int vel_coletavel = 7;


    map<string, int> contadores = {{"banana", 0}, {"camera", 0}, {"rosa", 0}};

    //flags pro autopulo, pra que ele pegue uma de cada
    bool pegou_rosa = false, pegou_banana = false, pegou_camera = false;

    //cria os 3 coletaveis iniciais
    vector<ColetavelData> coletaveis;
    for (int contador = 0; contador < 3; contador++){
        vector<float> alturas_ocupadas; //alturas ja usadas pelos outros
        vector<float> xs_ocupados; //posicoes x ja usadas pelos outros
        for (auto& outro_coletavel : coletaveis) {
            alturas_ocupadas.push_back(outro_coletavel.rect.y);
            xs_ocupados.push_back(outro_coletavel.rect.x);
        }
        coletaveis.push_back(base_engine.gerar_coletavel(alturas_ocupadas, xs_ocupados, largura_tela));
    }

    //mostra os coletaveis gerados
    cout << "Coletaveis iniciais (gerados aleatoriamente):\n";
    for (int indice = 0; indice < (int)coletaveis.size(); indice++){
        cout << "  [" << (indice + 1) << "] tipo=" << coletaveis[indice].tipo
             << " x=" << (int)coletaveis[indice].rect.x
             << " altura=" << (int)coletaveis[indice].rect.y << "\n";
    }
    cout << "\nComecou a corrida!\n\n";

    Rect gisele_rect = gisele.get_rect(); //hitbox da gisele

    int proximo_marco = 100; //imprime a distancia de 100 em 100 metros

    //parte princiapl
    while (run) {
        float delta_time = 1.0f / 30.0f; //tempo de cada quadro (sem relogio/GUI)

        //auto-pulo, onde pula para desviar de um coletavel rente ao chao
        if (!finalizando && !gisele.get_esta_pulando()) {
            for (auto& coletavel : coletaveis) {
                bool rente_ao_chao = coletavel.rect.y >= 560; //altura 565 = nivel da gisele
                bool chegando = coletavel.rect.x > gisele.get_X() && coletavel.rect.x < gisele.get_X() + 90;
                //so pula se ja pegou um desse tipo; senao deixa colidir para coletar 1 de cada
                bool ja_pegou = (coletavel.tipo == "rosa" && pegou_rosa == true) || (coletavel.tipo == "banana" && pegou_banana == true) || (coletavel.tipo == "camera" && pegou_camera == true);
                if (rente_ao_chao && chegando && ja_pegou) {
                    gisele.jump();
                    cout << "gisele pulou para desviar de uma " << coletavel.tipo << "!\n";
                    break;
                }
            }
        }

        bool estava_pulando = gisele.get_esta_pulando();
        gisele.update_fisica(delta_time); 

        //mostra a fisica do pulo quadro a quadro
        if (gisele.get_esta_pulando()) {
            cout << "pulando y=" << (int)gisele.get_Y() << "\n";
        } else if (estava_pulando) {
            cout << "gisele aterrissou y=" << (int)gisele.get_Y() << "\n";
        }

        distancia_pixels += 7;                               
        int distancia_metros = distancia_pixels / pixels_por_metro; //converte para metros

        //marco de distancia
        if (distancia_metros >= proximo_marco){
            cout << proximo_marco << " metros percorridos\n";
            proximo_marco += 100;
        }

        if (meta_metros <= distancia_metros - 50){ //chegou perto da meta
            finalizando = true;
        }

        if (finalizando) {//cenario parou, gisele anda ate o fim
            gisele.setX(gisele.get_X() + 5);
            if (gisele.get_X() >= largura_tela){
                run = false;  
                transicao_final = true; 
            }
        }

        // movimentacao dos coletaveis (so enquanto o cenario rola)
        if (!finalizando){
            for (auto& coletavel : coletaveis){
                coletavel.rect.x -= vel_coletavel; //vem da direita para a esquerda

                if (coletavel.rect.x + coletavel.rect.largura < 0){ //saiu pela esquerda
                    vector<float> alturas_ocupadas;
                    vector<float> xs_ocupados;
                    for (auto& outro_coletavel : coletaveis){
                        if (&outro_coletavel != &coletavel){ //ignora ele mesmo
                            alturas_ocupadas.push_back(outro_coletavel.rect.y);
                            xs_ocupados.push_back(outro_coletavel.rect.x);
                        }
                    }
                    coletavel = base_engine.gerar_coletavel(alturas_ocupadas, xs_ocupados, largura_tela);
                }
            }
            gisele_rect = gisele.get_rect(); //atualiza a hitbox enquanto corre
        }

        //colisõss
        for (auto& coletavel : coletaveis){
            if (gisele_rect.colliderect(coletavel.rect)){ //bateu no coletavel
                if (coletavel.tipo == "rosa"){
                    Rosa::efeito_rosa(contadores);
                    pegou_rosa = true;
                } else if (coletavel.tipo == "banana"){
                    Banana::efeito_banana(contadores);
                    pegou_banana = true;
                } else if (coletavel.tipo == "camera"){
                    Camera::efeito_camera(contadores);
                    pegou_camera = true;
                }

                cout << "coletou " << coletavel.tipo << " | banana=" << contadores["banana"] << " camera=" << contadores["camera"] << " rosa=" << contadores["rosa"] << "\n";

                // gera um novo coletavel no lugar do que foi coletado
                vector<float> alturas_ocupadas;
                vector<float> xs_ocupados;
                for (auto& outro_coletavel : coletaveis){
                    if (&outro_coletavel != &coletavel){
                        alturas_ocupadas.push_back(outro_coletavel.rect.y);
                        xs_ocupados.push_back(outro_coletavel.rect.x);
                    }
                }
                coletavel = base_engine.gerar_coletavel(alturas_ocupadas, xs_ocupados, largura_tela);
            }
        }

        //fim do jogo
        if (contadores["banana"] >= 3 || contadores["camera"] >= 3){
            run = false;
            perdeu = true; //bananas ou cameras demais leva pro game over
        } else if (distancia_metros >= meta_metros - 50){
            finalizando = true;
        }

        if (perdeu){
            gisele.setWalking(false); //ela para de andar
            cout << "\ngame over\n";
            cout << "distancia: " << distancia_metros << " m\n";
            cout << "banana=" << contadores["banana"]
                 << "camera=" << contadores["camera"]
                 << "rosa=" << contadores["rosa"] << "\n";
            cout << "andando: " << (gisele.get_esta_andando() ? "sim" : "nao") << "\n";
            return false; //não joga de novo
        }

        if (transicao_final){
            cout << "\nvitoria\n";
            cout << "distancia: " << distancia_metros << " m\n";
            cout << "banana=" << contadores["banana"]
                 << "camera=" << contadores["camera"]
                 << "rosa=" << contadores["rosa"] << "\n";
            return false;
        }
    }

    return false;
}