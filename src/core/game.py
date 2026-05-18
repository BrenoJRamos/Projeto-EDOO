import pygame
from pygame.locals import *
from sys import exit
import random
import os

# inicializando pygame + criando tela + inicializando variáveis globais + aceleração gradual do cenário
altura_tela = 700
largura_tela = 900
dimensoes_tela = (largura_tela, altura_tela)
pygame.init()    
pygame.display.set_caption('Gisele Bundchen VS As Forças Do Mal')
tela = pygame.display.set_mode(dimensoes_tela)
relogio = pygame.time.Clock()
ACELERAR = pygame.USEREVENT + 1
pygame.time.set_timer(ACELERAR, 10000)

# importando classes
from cenarios.tela_inicial import TelaInicial
from cenarios.tela_final import TelaFinal
from cenarios.desfile import Desfile
from personagens.gisele import Gisele
from coletaveis.base import Base
from coletaveis.banana import Banana
from coletaveis.camera import Camera
from coletaveis.rosa import Rosa

# instanciando objetos para acessar a classe
base_engine = Base()
gisele = Gisele()
banana = Banana()
camera = Camera()
tela_inicial = TelaInicial(tela)

# carregando imagens
FONTE_HUD = pygame.font.Font(os.path.join('assets', 'fonte', 'fonte.ttf'), 20)
HUD_BG = pygame.image.load(os.path.join('assets', 'cenario', 'placa_contadores1.png')).convert_alpha()
HUD_BG = pygame.transform.scale(HUD_BG, (250, 110))
LINHA_CHEGADA = pygame.image.load(os.path.join('assets', 'cenario', 'passarela_chegada.png')).convert_alpha()
LINHA_CHEGADA = pygame.transform.scale(LINHA_CHEGADA, dimensoes_tela)

# carregando música de fundo
musica_fundo = pygame.mixer.music.load(os.path.join('assets', 'sons', 'bg_music.mp3'))
pygame.mixer.music.set_volume(0.30)
pygame.mixer.music.play(-1)

# efeitos sonoros
som_vitoria = pygame.mixer.Sound(os.path.join('assets', 'sons','win_sound.mp3'))
som_vitoria.set_volume(0.50)
som_derrota  = pygame.mixer.Sound(os.path.join('assets', 'sons', 'game_over_sound.mp3'))
som_derrota.set_volume(0.50)
som_rosa = pygame.mixer.Sound(os.path.join('assets', 'sons', 'coleta_rosa.wav'))
som_rosa.set_volume(0.50)
som_camera = pygame.mixer.Sound(os.path.join('assets', 'sons', 'efeito_flash.mp3'))
som_camera.set_volume(0.50)
som_banana = pygame.mixer.Sound(os.path.join('assets', 'sons', 'efeito_banana.wav'))
som_banana.set_volume(0.50)

def main():

    global run 
    run = True
    finalizando = False
    perdeu = False
    som_vitoria_played = False
    som_derrota_played = False
    gisele.player_y = 500 # ou a posição inicial dela
    gisele.player_x = 100
    gisele.esta_pulando = False
    transicao_final = False
    pygame.mixer.music.set_volume(0.30)
    pygame.mixer.music.play(-1)

    # configurando cenário
    tela_inicial.rodar()
    bg_image, bg_width, tiles, scroll = Desfile.iniciar_passarela(dimensoes_tela, largura_tela)

    # definindo distância
    distancia_pixels = 0
    pixels_por_metro = 20
    meta_metros = 700
    vel_coletavel = 7

    # dicionários com os contadores
    contadores = {
        "banana": 0,   
        "camera": 0,   
        "rosa": 0    
    }

    coletaveis = []

    for _ in range(3):
        alturas_ocupadas = [c["rect"].y for c in coletaveis]
        xs_ocupados = [c["rect"].x for c in coletaveis]
        sprite_coletavel = random.choice(base_engine.sprites_coletaveis)
        coletaveis.append(
            base_engine.gerar_coletavel(
                sprite_coletavel,
                alturas_ocupadas,
                xs_ocupados,
                largura_tela
            )
        )

    # loop principal
    while run:
        # variaveis locais
        delta_time = relogio.tick(30) / 1000.0 # tempo decorrido em segundos
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
                 run = False # fim do jogo / tela de vitória
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

        # colisões
        for coletavel in coletaveis:
            if gisele_rect.colliderect(coletavel["rect"]):
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

        # mostrando os contadores na tela
        for coletavel in coletaveis:
            tela.blit(coletavel["coletavel"], (coletavel["rect"]))

        # mostrando gisele na tela
        gisele.desenhar(tela)

        # HUD 
        fundo_x = 20
        fundo_y = 8

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

        # flash do paparazzi
        camera.desenhar_flash(tela, dimensoes_tela)

        # fim do jogo
        if contadores["banana"] >= 3 or contadores["camera"] >= 3:
            run = False
            perdeu = True
            # tela de game over
        elif distancia_metros >= meta_metros-50:
            finalizando = True
        pygame.display.update()

        if perdeu:
            pygame.mixer.music.stop()
            if not som_derrota_played:
                som_derrota.play()
                som_derrota_played = True
            # Chame o método da tela final que exibe derrota
            tela_final = TelaFinal(tela, contadores, distancia_metros, meta_metros)
            return tela_final.run() # retornar True para reiniciar ou False para sair
                
        if finalizando:
            pygame.mixer.music.stop()
            if not som_vitoria_played:
                som_vitoria.play()
                som_vitoria_played = True
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