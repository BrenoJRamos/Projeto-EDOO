# 📚 Guia de Estudos — Gisele Bündchen vs As Forças do Mal (C++)

Guia do projeto da disciplina **EDOO (Estrutura de Dados Orientada a Objetos)**.
Explica o que cada arquivo faz, o que cada parte do código faz e **quais conceitos de
Programação Orientada a Objetos (POO)** foram usados e **onde**.

---

## 1. Visão geral

O jogo é uma **simulação automática** (sem GUI e sem input do jogador) do jogo original
em Python. A "Gisele" corre por uma passarela enquanto coletáveis (banana, câmera, rosa)
vêm da direita para a esquerda. A lógica decide quando pular e o que coletar, e o resultado
(vitória ou game over) é impresso no terminal.

**Regras principais:**
- A meta é percorrer ~500 metros.
- Pegar **3 bananas** ou **3 câmeras** → *game over*.
- Chegar ao fim da passarela → *vitória*.
- A banana zera o contador de rosas (efeito especial).

---

## 2. Arquitetura e fluxo

```text
src/
├── main.cpp                      → ponto de entrada (função main)
├── core/
│   ├── Game.hpp / Game.cpp       → o "cérebro": loop e regras do jogo
├── player/
│   ├── Player.hpp / Player.cpp   → a personagem (posição, física, pulo)
└── collectibles/
    ├── Collectible.hpp / .cpp    → Rect (colisão), ColetavelData, Base (gerador)
    ├── Banana.hpp                → coletável banana (herda de Base)
    ├── Camera.hpp                → coletável câmera (herda de Base)
    └── Rosa.hpp                  → coletável rosa  (herda de Base)
```

**Como tudo se conecta:**

```text
main()  ──cria──▶  Game  ──tem um──▶  Player (gisele)
                     │
                     └──tem um──▶  Base (base_engine)  ──gera──▶  ColetavelData (usa Rect)

Banana / Camera / Rosa  ──herdam──▶  Base
```

`main` cria um `Game`, que **contém** um `Player` e uma `Base`. A `Base` produz os
coletáveis. As colisões usam o `Rect`. Os efeitos de cada item vêm das classes filhas
de `Base` (Banana, Camera, Rosa).

---

## 3. Arquivo por arquivo

### 3.1 `main.cpp` — ponto de entrada

```cpp
#include <Game.hpp>

int main() {
    Game game;                       // cria o objeto do jogo
    while (true) {
        bool jogar_novamente = game.jogo();   // roda uma partida
        if (jogar_novamente == false) {
            break;                   // sai se não for jogar de novo
        }
    }
    return 0;
}
```

- **`Game game;`** → cria um **objeto** (instância) da classe `Game`. Aqui o construtor
  `Game()` é chamado automaticamente.
- **`game.jogo()`** → chama o método que roda uma partida inteira. Retorna `bool`:
  `true` = jogar de novo, `false` = encerrar.
- O `while (true)` permitiria repetir partidas; como `jogo()` sempre retorna `false`,
  na prática roda uma vez e termina.

> **Conceito:** *instanciação de objeto* e *chamada de método*.

---

### 3.2 `core/Game.hpp` — a interface da classe Game

```cpp
#ifndef GAME_HPP          // guarda de inclusão (include guard)
#define GAME_HPP

#include <Collectible.hpp>
#include <Player.hpp>

class Game {
public:
    Game();               // construtor
    bool jogo();          // roda uma partida; true = jogar de novo

private:
    int altura_tela;      // atributos privados (encapsulados)
    int largura_tela;
    Player gisele;        // COMPOSIÇÃO: Game "tem um" Player
    Base base_engine;     // COMPOSIÇÃO: Game "tem uma" Base
};

#endif
```

- **`#ifndef / #define / #endif`** → *include guard*. Impede que o mesmo cabeçalho seja
  incluído duas vezes (evita erro de redefinição).
- **`public:`** → o que é visível de fora (construtor e o método `jogo()`).
- **`private:`** → o que fica escondido (os atributos). Só o próprio `Game` acessa.
- **`Player gisele;` e `Base base_engine;`** → o `Game` **contém** outros objetos.

> **Conceitos:** *encapsulamento* (private/public), *composição* (Game tem Player e Base),
> *separação interface (.hpp) × implementação (.cpp)*.

---

### 3.3 `core/Game.cpp` — a lógica do jogo

Este é o arquivo mais importante. Vamos por partes.

**Construtor — define o tamanho da tela:**
```cpp
Game::Game(){
    altura_tela = 700;
    largura_tela = 900;
}
```
`Game::Game` significa "o construtor `Game` que pertence à classe `Game`" (o `::` é o
**operador de resolução de escopo**).

**Início da partida — estados e posição inicial:**
```cpp
bool run = true;              // jogo rodando
bool finalizando = false;    // chegou perto do fim
bool perdeu = false;         // game over
bool transicao_final = false;// venceu

gisele.setPosition(100, 500); // usa um SETTER do Player
```
Aqui o `Game` **comanda** o `Player` por meio de métodos públicos — não mexe direto nos
atributos da Gisele (eles são privados).

**Mostra a ficha usando GETTERS:**
```cpp
cout << "posicao inicial: x=" << gisele.get_X() << " y=" << gisele.get_Y() << "\n";
cout << "hitbox: largura=" << gisele.get_largura() << " altura=" << gisele.get_altura() << "\n";
```
Os `get_*()` leem os atributos privados de forma controlada → **encapsulamento na prática**.

**Contadores e flags:**
```cpp
map<string, int> contadores = {{"banana", 0}, {"camera", 0}, {"rosa", 0}};

// flags do auto-pulo: garantem que ela pega 1 de cada tipo
bool pegou_rosa = false, pegou_banana = false, pegou_camera = false;
```
- `contadores` → quantos de cada tipo foram coletados (usa `std::map`).
- `pegou_*` → flags booleanas que controlam **só o auto-pulo** (separadas do `contadores`,
  pois a banana zera o contador de rosa, mas a flag deve permanecer `true`).

**Criação dos 3 coletáveis iniciais:**
```cpp
vector<ColetavelData> coletaveis;
for (int contador = 0; contador < 3; contador++){
    vector<float> alturas_ocupadas;
    vector<float> xs_ocupados;
    for (auto& outro_coletavel : coletaveis) {      // olha os já criados
        alturas_ocupadas.push_back(outro_coletavel.rect.y);
        xs_ocupados.push_back(outro_coletavel.rect.x);
    }
    coletaveis.push_back(base_engine.gerar_coletavel(alturas_ocupadas, xs_ocupados, largura_tela));
}
```
Pede para a `base_engine` (objeto `Base`) gerar cada coletável **sem sobrepor** os outros.
Usa `std::vector` para guardar a lista.

**Loop principal `while (run)`** — roda quadro a quadro (1/30 s por quadro).

1. **Auto-pulo:**
```cpp
if (!finalizando && !gisele.get_esta_pulando()) {
    for (auto& coletavel : coletaveis) {
        bool rente_ao_chao = coletavel.rect.y >= 560;
        bool chegando = coletavel.rect.x > gisele.get_X() && coletavel.rect.x < gisele.get_X() + 90;
        // só pula se já pegou um desse tipo; senão deixa colidir para coletar 1 de cada
        bool ja_pegou = (coletavel.tipo == "rosa"   && pegou_rosa   == true) ||
                        (coletavel.tipo == "banana" && pegou_banana == true) ||
                        (coletavel.tipo == "camera" && pegou_camera == true);
        if (rente_ao_chao && chegando && ja_pegou) {
            gisele.jump();          // manda a Gisele pular
            break;
        }
    }
}
```
Enquanto a flag do tipo for `false`, ela **não pula** e deixa colidir (coletando 1 daquele
tipo). Depois que `pegou_*` vira `true`, volta a pular naquele tipo.

2. **Física do pulo:**
```cpp
bool estava_pulando = gisele.get_esta_pulando();
gisele.update_fisica(delta_time);   // aplica gravidade/pulo
```

3. **Distância e marcos:** `distancia_pixels += 7`, converte para metros e imprime de 100 em 100.

4. **Finalização:** quando perto da meta, o cenário "para" e a Gisele anda até o fim da tela.

5. **Movimento dos coletáveis:** vêm para a esquerda; quando saem da tela, são regenerados.

6. **Colisões:**
```cpp
for (auto& coletavel : coletaveis){
    if (gisele_rect.colliderect(coletavel.rect)){   // bateu?
        if (coletavel.tipo == "rosa"){
            Rosa::efeito_rosa(contadores);    pegou_rosa   = true;
        } else if (coletavel.tipo == "banana"){
            Banana::efeito_banana(contadores); pegou_banana = true;
        } else if (coletavel.tipo == "camera"){
            Camera::efeito_camera(contadores); pegou_camera = true;
        }
        ...
        coletavel = base_engine.gerar_coletavel(...); // gera outro no lugar
    }
}
```
`colliderect` (do `Rect`) detecta a batida; o efeito de cada item vem da **classe filha**
correspondente (Rosa/Banana/Camera). O tipo é decidido com `if/else` sobre a string `tipo`.

7. **Fim do jogo:**
```cpp
if (contadores["banana"] >= 3 || contadores["camera"] >= 3){ perdeu = true; ... }
...
if (perdeu)         { cout << "game over"; return false; }
if (transicao_final){ cout << "vitoria";  return false; }
```

> **Conceitos:** *encapsulamento* (acessa o Player só por getters/setters),
> *colaboração entre objetos* (Game ↔ Player ↔ Base), *uso de STL* (`vector`, `map`, `string`),
> *referências* (`auto&` no for-each).

---

### 3.4 `player/Player.hpp` — interface da personagem

```cpp
class Player {
private:
    float x, y;                       // posição
    float largura, altura;            // hitbox
    float velocidadeX, velocidadeY;   // física
    float gravidade, forca_pulo;
    bool esta_pulando, esta_andando;  // estados
    float chao;

public:
    Player();                         // construtor
    void jump();                      // ações
    void update_fisica(float deltaTime);
    Rect get_rect() const;            // hitbox atual

    // getters (leem atributos privados)
    float get_X() const;
    float get_Y() const;
    bool  get_esta_pulando() const;
    // ... outros getters ...

    // setters (alteram atributos privados de forma controlada)
    void setPosition(float newX, float newY);
    void setWalking(bool walking);
    void setX(int novox);
};
```

- **Todos os dados são `private`** → ninguém de fora altera `x`, `y`, etc. diretamente.
- O acesso é só pelos **getters/setters** públicos. Esse é o exemplo **mais claro de
  encapsulamento** do projeto.
- **`const`** nos getters → indica que o método **não altera** o objeto.

> **Conceitos:** *encapsulamento forte*, *const-correctness*, *abstração* (de fora você
> usa `jump()` sem saber como a física funciona por dentro).

---

### 3.5 `player/Player.cpp` — implementação da personagem

**Construtor — valores iniciais:**
```cpp
Player::Player() {
    x = 120.0f; y = 525.0f;
    largura = 50.0f; altura = 70.0f;
    velocidadeX = 8.0f; velocidadeY = 0.0f;
    gravidade = 2000.0f;
    forca_pulo = -840.0f;
    esta_pulando = false; esta_andando = true;
    chao = 525.0f;
}
```

**Pulo — só pula se não estiver pulando:**
```cpp
void Player::jump() {
    if (!esta_pulando) {
        velocidadeY = forca_pulo;  // impulso para cima (negativo)
        esta_pulando = true;
    }
}
```

**Física — gravidade + colisão com o chão:**
```cpp
void Player::update_fisica(float deltaTime) {
    velocidadeY += gravidade * deltaTime;  // gravidade puxa para baixo
    y += velocidadeY * deltaTime;          // move verticalmente
    if (y >= chao) {                       // bateu no chão?
        y = chao;
        velocidadeY = 0.0f;
        esta_pulando = false;
    }
}
```
> No eixo Y da tela, **para baixo é positivo**. Por isso `forca_pulo` é **negativo** (sobe)
> e a gravidade é positiva (desce).

**`get_rect()` — devolve a hitbox como um `Rect`:**
```cpp
Rect Player::get_rect() const {
    return Rect{x, y, largura, altura};
}
```
Isso conecta o `Player` ao sistema de colisão sem expor os atributos.

> **Conceitos:** *construtor inicializando o estado*, *abstração da física*,
> *encapsulamento* (o `Rect` é montado internamente).

---

### 3.6 `collectibles/Collectible.hpp` — Rect, ColetavelData e Base

**`struct Rect` — retângulo + colisão (AABB):**
```cpp
struct Rect {
    float x, y, largura, altura;

    bool colliderect(const Rect& outro) const {
        return x < outro.x + outro.largura && x + largura > outro.x &&
               y < outro.y + outro.altura && y + altura > outro.y;
    }
};
```
- **AABB** = *Axis-Aligned Bounding Box* (colisão de retângulos alinhados aos eixos).
- A condição verifica se há sobreposição nos eixos X **e** Y ao mesmo tempo.

**`struct ColetavelData` — dados de um coletável:**
```cpp
struct ColetavelData {
    Rect rect;                   // posição/tamanho
    std::string tipo = "banana"; // "banana", "camera" ou "rosa"
};
```

**`class Base` — o gerador (engine) de coletáveis:**
```cpp
class Base {
public:
    int tamanho_coletavel = 40;
    std::vector<int> alturas_coletaveis = {565, 480, 460};
    std::vector<std::string> tipos_coletaveis = {"banana", "camera", "rosa"};
    float distancia_minima_x = 250;

    Base();
    ColetavelData gerar_coletavel(const std::vector<float>& alturas_ocupadas,
                                  const std::vector<float>& xs_ocupados,
                                  int largura_tela);
};
```

> **Conceitos:** *abstração* (o `Rect` esconde a matemática da colisão num método),
> *struct vs class* (struct = membros públicos por padrão; class = privado por padrão),
> *valores default em membros*.

---

### 3.7 `collectibles/Collectible.cpp` — geração aleatória

```cpp
ColetavelData Base::gerar_coletavel(...) {
    static std::mt19937 gen(std::random_device{}()); // gerador aleatório (1x só)

    // 1) escolhe uma ALTURA que não esteja ocupada (se possível)
    std::vector<int> alturas_disponiveis;
    for (int altura_possivel : alturas_coletaveis) {
        if (std::find(alturas_ocupadas.begin(), alturas_ocupadas.end(),
                      (float)altura_possivel) == alturas_ocupadas.end()) {
            alturas_disponiveis.push_back(altura_possivel);
        }
    }
    // ... sorteia entre as livres (ou qualquer uma se todas ocupadas) ...

    // 2) escolhe uma posição X espaçada das outras (até 11 tentativas)
    for (int tentativas = 0; tentativas < 11; ++tentativas) {
        pos_x = (float)disX(gen);
        // verifica distancia_minima_x para os outros...
    }

    // 3) sorteia o TIPO e monta o coletável
    coletavel.rect = {pos_x, (float)altura, ...};
    coletavel.tipo = tipos_coletaveis[disTipo(gen)];
    return coletavel;
}
```
- **`std::mt19937` + `std::random_device`** → geração de números aleatórios moderna do C++.
- **`static`** no gerador → criado **uma única vez**, reaproveitado em todas as chamadas.
- **`std::find`** (da `<algorithm>`) → verifica se uma altura já está ocupada.

> **Conceitos:** *abstração* (quem chama só pede "gere um coletável" e não vê o sorteio),
> *uso de STL/algoritmos*, *responsabilidade única* (a Base só cuida de gerar coletáveis).

---

### 3.8 `Banana.hpp`, `Camera.hpp`, `Rosa.hpp` — os coletáveis (HERANÇA)

**Rosa:**
```cpp
class Rosa : public Base {            // HERANÇA: Rosa é uma Base
public:
    static void efeito_rosa(std::map<std::string, int>& contadores) {
        contadores["rosa"] += 1;
    }
};
```

**Banana** (com efeito especial — zera a rosa):
```cpp
class Banana : public Base {
public:
    static void efeito_banana(std::map<std::string, int>& contadores) {
        contadores["rosa"] = 0;       // efeito especial!
        contadores["banana"] += 1;
    }
};
```

**Camera** (tem estado e métodos extras do efeito flash):
```cpp
class Camera : public Base {
public:
    bool flash = false;
    float flash_raio = 0;
    // ...
    static void efeito_camera(std::map<std::string, int>& contadores) {
        contadores["camera"] += 1;
    }
    void iniciar_flash(int largura_tela, int altura_tela) { ... }
    void atualizar_logica_flash() { ... }
};
```

- **`class Banana : public Base`** → cada coletável **herda** de `Base` (relação "é um").
- O parâmetro **`std::map<...>& contadores`** é passado **por referência** (`&`): o método
  altera o mapa original, não uma cópia.
- Os efeitos são **`static`**: chamados pelo nome da classe (`Rosa::efeito_rosa(...)`),
  sem precisar de um objeto.

> **Conceitos:** *herança* (Banana/Camera/Rosa **herdam** de Base), *métodos estáticos*,
> *passagem por referência*, *especialização* (cada filha define seu próprio efeito).

---

## 4. Conceitos de POO — resumo e ONDE estão

| Conceito | O que é | Onde no projeto |
|---|---|---|
| **Classe e Objeto** | Molde × instância | `class Game/Player/Base`; `Game game;` em `main.cpp` |
| **Encapsulamento** | Esconder dados; acesso via métodos | `Player` (tudo `private` + getters/setters); `Game` (atributos `private`) |
| **Abstração** | Usar sem conhecer o "como" | `gisele.jump()`, `Rect::colliderect()`, `base_engine.gerar_coletavel()` |
| **Herança** | Relação "é um" | `Banana/Camera/Rosa : public Base` (nos `.hpp` dos coletáveis) |
| **Composição** | Relação "tem um" | `Game` tem `Player gisele` e `Base base_engine` (`Game.hpp`) |
| **Construtor** | Inicializa o objeto | `Game::Game()`, `Player::Player()`, `Base::Base()` |
| **Métodos const** | Não alteram o objeto | getters de `Player` (`get_X() const`, etc.) |
| **Métodos estáticos** | Pertencem à classe, não ao objeto | `Rosa::efeito_rosa`, `Banana::efeito_banana`, `Camera::efeito_camera` |
| **struct × class** | Default público × privado | `struct Rect/ColetavelData` × `class Game/Player/Base` |

### Sobre polimorfismo

O projeto usa **herança**, mas **não usa polimorfismo de tempo de execução** (não há
funções `virtual` nem chamadas pela classe base). O tipo de cada coletável é decidido
com `if/else` sobre a string `tipo`, no bloco de colisão do `Game.cpp`:

```cpp
if (coletavel.tipo == "rosa")        Rosa::efeito_rosa(contadores);
else if (coletavel.tipo == "banana") Banana::efeito_banana(contadores);
else if (coletavel.tipo == "camera") Camera::efeito_camera(contadores);
```

---

## 5. Conceitos de C++ / EDOO que aparecem

- **STL (Standard Template Library):** `std::vector`, `std::map`, `std::string`.
- **Referências (`&`):** `for (auto& coletavel : coletaveis)` evita copiar; parâmetros
  `const std::vector<float>&` passam sem copiar.
- **`auto`:** dedução automática de tipo nos `for`-each.
- **Aleatoriedade moderna:** `std::mt19937`, `std::random_device`, `std::uniform_int_distribution`.
- **Include guards:** `#ifndef/#define/#endif` em todos os `.hpp`.
- **Separação `.hpp` (declara) × `.cpp` (implementa).**

---

## 6. Como compilar e rodar

A partir da pasta `Projeto-EDOO/src`:

```bash
clang++ -std=c++17 main.cpp core/Game.cpp player/Player.cpp collectibles/Collectible.cpp -I core -I player -I collectibles -o jogo && ./jogo
```

- Os `.hpp` dos coletáveis (Banana/Camera/Rosa) **não** entram no comando: são só
  cabeçalhos, incluídos pelo `Game.cpp`.
- `-I core -I player -I collectibles` informa onde achar os cabeçalhos (`#include <...>`).
- `-o jogo` define o nome do executável; `./jogo` roda.

---

*Guia gerado para fins de estudo da disciplina EDOO.*
