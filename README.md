# Gisele Bündchen vs As Forças do Mal (C++)

Projeto da disciplina Estrutura de Dados Orientada a Objetos (EDOO) do CIn/UFPE.
O grupo escolheu portar o jogo "Gisele Bündchen vs As Forças do Mal" (originalmente
escrito em Python com Pygame) para C++ puro, sem dependências externas, com
foco em modelagem OO. A versão C++ roda inteiramente no terminal, com renderização
ASCII animada e input não-bloqueante.

---

## Sobre o Jogo

Side-scroller runner: Gisele corre automaticamente pela passarela. O jogador
pressiona SPACE (ou W) para pular. O cenário cospe três tipos de coletáveis:
bananas (B) e câmeras (C) contam como hit, rosas (R) somam pontos. Levou
3 hits, perdeu. Chegou ao fim da passarela, ganhou. A cada 10 segundos os
coletáveis ficam mais rápidos, até travar em uma velocidade máxima.

---

## Arquitetura

| Classe | Arquivo | Responsabilidade |
|--------|---------|------------------|
| Rect | utils/Rect.hpp | Hitbox AABB com detecção de sobreposição |
| Vector2 | utils/Vector2.hpp | Vetor 2D com operações matemáticas |
| Stats | utils/Stats.hpp | Contadores de jogo e condições de fim |
| Player | player/Player.hpp/cpp | Física da Gisele (gravidade, pulo, posição) |
| Base | collectibles/Collectible.hpp/cpp | Fábrica de coletáveis com spawn sem sobreposição |
| Banana | collectibles/Banana.hpp | Efeito de colisão: banana++ e reset de rosas |
| Camera | collectibles/Camera.hpp | Efeito de colisão: camera++ e flash visual |
| Rosa | collectibles/Rosa.hpp | Efeito de colisão: rosa++ |
| CollisionSystem | systems/CollisionSystem.hpp/cpp | Detecção AABB entre Player e coletáveis |
| Spawner | systems/Spawner.hpp/cpp | Movimento, respawn e aceleração dos coletáveis |
| GameState | core/GameState.hpp | Enum com estados (RODANDO, FINALIZANDO, VITORIA, DERROTA) |
| Game | core/Game.hpp/cpp | Loop principal: update, física, transição de estados |
| TerminalRenderer | core/TerminalRenderer.hpp/cpp | Render ASCII, input não-bloqueante, telas de UI |

---

## Requisitos

### Linux / WSL2 (Ubuntu)

- g++ com suporte a C++17
- Instalar se necessário:
  ```bash
  sudo apt update && sudo apt install -y build-essential
  ```
- Verificar:
  ```bash
  g++ --version   # precisa ser >= 9.0
  ```

### Windows (MinGW via MSYS2)

- Instalar MSYS2: https://www.msys2.org
- No terminal MSYS2 MinGW64:
  ```bash
  pacman -S mingw-w64-x86_64-gcc
  ```
- Verificar:
  ```bash
  g++ --version
  ```
- Observação: no Windows nativo o modo raw do terminal pode ter comportamento
  diferente. Recomendado rodar via WSL2 para melhor experiência.

---

## Como Compilar

Os flags `-I` apontam pro g++ onde achar os headers do projeto. Nenhuma
biblioteca externa é necessária, só a stdlib.

### Linux / WSL2

```bash
g++ -std=c++17 \
    src/player/Player.cpp \
    src/collectibles/Collectible.cpp \
    src/systems/CollisionSystem.cpp \
    src/systems/Spawner.cpp \
    src/core/Game.cpp \
    src/core/TerminalRenderer.cpp \
    src/main.cpp \
    -Isrc \
    -Isrc/utils \
    -Isrc/player \
    -Isrc/collectibles \
    -Isrc/systems \
    -Isrc/core \
    -o gisele_game
```

### Windows (MinGW, terminal MSYS2 MinGW64)

```bash
g++ -std=c++17 \
    src/player/Player.cpp \
    src/collectibles/Collectible.cpp \
    src/systems/CollisionSystem.cpp \
    src/systems/Spawner.cpp \
    src/core/Game.cpp \
    src/core/TerminalRenderer.cpp \
    src/main.cpp \
    -Isrc \
    -Isrc/utils \
    -Isrc/player \
    -Isrc/collectibles \
    -Isrc/systems \
    -Isrc/core \
    -o gisele_game.exe
```

---

## Como Executar

### Linux / WSL2

```bash
./gisele_game
```

### Windows

```bash
./gisele_game.exe
```

Ao iniciar o programa imprime a arquitetura (todas as classes), depois entra
na tela de início. Pressione SPACE para começar. Durante a partida, SPACE ou
W pula e Q sai a qualquer momento.

---

## Controles

| Tecla | Ação |
|-------|------|
| SPACE | Pular |
| W | Pular (alternativo) |
| Q | Sair do jogo |
| S / N | Jogar de novo (na tela final) |

---

## Integrantes

- Thiago José Barbosa Menezes de Oliveira (tjbmo)
- Breno José Ramos da Silva (bjrs)
- Leonardo Gonçalves Sobral (lgs5)

---

## Disciplina

Este projeto foi realizado para a disciplina Estrutura de Dados Orientada a
Objetos (EDOO), aplicando conceitos como:

- Abstração
- Encapsulamento
- Herança e polimorfismo
- Modularização
- Organização e estruturação de projetos em C++

---

## Licença

Este projeto é de caráter acadêmico e possui fins exclusivamente educacionais.
