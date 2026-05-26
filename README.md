# Gisele Bündchen vs As Forças do Mal (C++)

Projeto da disciplina **Estrutura de Dados Orientada a Objetos (EDOO)** do CIn/UFPE.
O grupo escolheu portar o jogo "Gisele Bündchen vs As Forças do Mal" (originalmente
escrito em Python) para **C++ puro**, sem dependências externas, com foco em modelagem
orientada a objetos.

A versão C++ é uma **simulação de terminal**: o jogo roda sozinho (sem entrada do
jogador) e imprime no console o que vai acontecendo — a ficha da personagem, os
coletáveis gerados, os pulos, a distância percorrida e o resultado final.

---

## Como Compilar e Executar

Requer um compilador com suporte a **C++17** (`g++`). Nenhuma biblioteca externa é
necessária — apenas a biblioteca padrão.

A partir da pasta raiz do projeto (`Projeto-EDOO`), o comando abaixo compila e já executa:

```bash
g++ -std=c++17 src/main.cpp src/core/Game.cpp src/player/Player.cpp src/collectibles/Collectible.cpp -Isrc/core -Isrc/player -Isrc/collectibles -o jogo && ./jogo
```

Depois de compilado, para rodar de novo basta:

```bash
./jogo
```

Detalhes do comando:
- Apenas os 4 arquivos `.cpp` são compilados (os `.hpp` entram pelos `#include`).
- `-Isrc/core -Isrc/player -Isrc/collectibles` indicam onde achar os headers.
- `-o jogo` define o nome do executável; o `&& ./jogo` roda só se a compilação der certo.

> Em outros sistemas, o mesmo comando vale trocando o nome do executável (ex.: `-o jogo.exe`).

---

## Sobre o Jogo

A Gisele corre automaticamente pela passarela enquanto três tipos de coletáveis vêm
da direita para a esquerda:

- **Banana** — conta como hit e zera o contador de rosas.
- **Câmera** — conta como hit.
- **Rosa** — soma pontos.

A lógica controla os pulos automaticamente (a Gisele coleta um de cada tipo e depois
passa a pular para desviar). O jogo termina de duas formas:

- **Game over** — ao acumular 3 bananas ou 3 câmeras.
- **Vitória** — ao chegar ao fim da passarela (~500 metros).

---

## Estrutura do Projeto

```text
src/
├── main.cpp                     → ponto de entrada (função main)
├── core/
│   ├── Game.hpp                 → interface da classe Game
│   └── Game.cpp                 → loop principal e regras da simulação
├── player/
│   ├── Player.hpp               → interface da personagem
│   └── Player.cpp               → física da Gisele (gravidade, pulo, posição)
└── collectibles/
    ├── Collectible.hpp          → Rect, ColetavelData e a classe Base
    ├── Collectible.cpp          → geração aleatória de coletáveis
    ├── Banana.hpp               → coletável Banana (herda de Base)
    ├── Camera.hpp               → coletável Camera (herda de Base)
    └── Rosa.hpp                 → coletável Rosa  (herda de Base)
```

---

## Arquitetura

| Classe / Tipo | Arquivo | Responsabilidade |
|---|---|---|
| Rect | collectibles/Collectible.hpp | Hitbox AABB com detecção de colisão (`colliderect`) |
| ColetavelData | collectibles/Collectible.hpp | Dados de um coletável (posição + tipo) |
| Base | collectibles/Collectible.hpp/.cpp | Geração de coletáveis sem sobreposição + efeito (virtual) |
| Banana | collectibles/Banana.hpp | Efeito: `banana++` e zera as rosas |
| Camera | collectibles/Camera.hpp | Efeito (estático): `camera++` e lógica de flash |
| Rosa | collectibles/Rosa.hpp | Efeito: `rosa++` |
| Player | player/Player.hpp/.cpp | Física da Gisele (gravidade, pulo, posição, hitbox) |
| Game | core/Game.hpp/.cpp | Loop principal: auto-pulo, movimento, colisões e estados |

---

## Conceitos de POO aplicados

- **Encapsulamento** — atributos privados no `Player` e no `Game`, acessados por getters/setters.
- **Herança** — `Banana`, `Camera` e `Rosa` herdam de `Base`.
- **Polimorfismo** — método `efeito` virtual na `Base`, sobrescrito (`override`) em `Banana`
  e `Rosa` e chamado por referência da base (`Base&`).
- **Composição** — o `Game` contém um `Player` e uma `Base`.
- **Construtor e destrutor** — incluindo destrutores em `Banana` e `Rosa`.
- **Sobrecarga de operador** — `operator<<` para imprimir um `Player`.
- **Métodos estáticos** — efeito da `Camera`.
- **Outros** — métodos e parâmetros `const`, passagem por referência, parâmetro com valor
  padrão, ponteiro para objeto e uso de containers da STL (`vector`, `map`).

---

## Integrantes

- Thiago José Barbosa Menezes de Oliveira (tjbmo)
- Breno José Ramos da Silva (bjrs)
- Leonardo Gonçalves Sobral (lgs5)
- João Carlos Melo Brennand de Souza Mendes (jcmbsm)

---

## Disciplina

Este projeto foi realizado para a disciplina **Estrutura de Dados Orientada a Objetos
(EDOO)**, aplicando conceitos como:

- Abstração
- Encapsulamento
- Herança e polimorfismo
- Modularização
- Organização e estruturação de projetos em C++

---

## Licença

Este projeto é de caráter acadêmico e possui fins exclusivamente educacionais.
