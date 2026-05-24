# Guia de Estudo: Gisele Bündchen vs As Forças do Mal (C++)
## Apresentação EDOO | Leonardo Gonçalves Sobral

Esse arquivo é meu mapa pra apresentação. Cobre cada classe do projeto, o porquê
das decisões e as perguntas que o professor provavelmente vai fazer. Foco total
nos arquivos que eu escrevi, mas preciso saber explicar tudo, inclusive o que o
Thiago e o Breno entregaram.

---

## Visão Geral da Arquitetura

O projeto é organizado em 4 camadas. `utils/` tem utilitários puros (Rect,
Vector2, Stats). `player/` e `collectibles/` são as entidades do jogo.
`systems/` tem CollisionSystem e Spawner, classes que operam SOBRE as entidades
mas não são entidades. `core/` tem o loop principal (Game), o estado
(GameState) e a camada de apresentação (TerminalRenderer).

O fluxo de dados é uma via só: `main` cria um `TerminalRenderer`, que herda
de `Game`. `Game` é dono de tudo: Player, Camera, CollisionSystem, Spawner,
Stats e o vector de coletáveis. A cada frame, `Game::rodar()` chama
`processarInput()`, `atualizar()`, `checarEstado()` e `renderizar()`, nessa
ordem. O Spawner move e respawna os coletáveis, o CollisionSystem checa
sobreposição AABB e dispara o efeito certo (Banana / Camera / Rosa), que
mexe no Stats. O TerminalRenderer só lê o estado e desenha.

Por que esse desenho faz sentido em OOP: cada classe tem uma responsabilidade
única, as dependências apontam pra dentro (utilitários não conhecem entidades,
entidades não conhecem sistemas, sistemas não conhecem o renderer). É fácil
trocar peça por peça sem quebrar o resto.

```
main.cpp
└── TerminalRenderer (herda Game)
    ├── Game
    │   ├── Player
    │   ├── CollisionSystem
    │   │   ├── Rect (AABB)
    │   │   ├── Banana / Camera / Rosa (efeitos)
    │   │   └── Stats (contadores)
    │   ├── Spawner
    │   │   └── Base (gera ColetavelData)
    │   └── Stats
    └── GameState (enum)
```

---

## Seção por Classe

### utils/Rect.hpp (Léo)

**O que é:** retângulo de hitbox com teste de colisão AABB.
**Onde está:** `src/utils/Rect.hpp`.
**Quem escreveu:** eu.

**O que eu preciso saber explicar:**
Virou classe em vez de struct porque o professor cobrou encapsulamento. Campos
x, y, w, h ficam privados, getters e setters expostos. `colliderect()` é o
método AABB: duas caixas se sobrepõem se `a.x < b.x + b.w` E `a.x + a.w > b.x`
E o mesmo pro eixo Y. As quatro condições juntas garantem que existe pelo
menos um ponto comum.

Tem `getRight()` e `getBottom()` por conveniência: evita escrever `x + w` em
todo lugar e deixa o `colliderect` mais legível.

```
[ A ][ B ]      <- B começa onde A termina, sem overlap
[ A ]
   [ B ]        <- overlap no X
```

**Perguntas prováveis:**
- **P: Por que classe e não struct?** R: Encapsulamento, deixei x/y/w/h
  privados e exponho só getters/setters. Struct ia deixar tudo public, perde
  o controle.
- **P: Como o AABB funciona?** R: É o teste de sobreposição de dois retângulos
  alinhados aos eixos. Se em qualquer um dos eixos eles NÃO se sobrepõem, não
  bateu. Se sobrepõem nos dois eixos, bateu.

---

### utils/Vector2.hpp (Léo)

**O que é:** vetor 2D com operadores e operações matemáticas.
**Onde está:** `src/utils/Vector2.hpp`.
**Quem escreveu:** eu.

**O que eu preciso saber explicar:**
O Player não usa muito porque trabalha com float cru, mas a classe existe pra
mostrar sobrecarga de operadores e ficar disponível pra qualquer extensão. `operator+`
retorna um Vector2 novo, não modifica os originais (semântica de valor).
`magnitude()` usa Pitágoras (`sqrt(x*x + y*y)`). `normalize()` divide cada
componente pela magnitude, dá um vetor de tamanho 1 na mesma direção. Trato o
caso de vetor zero pra não dividir por zero.

**Perguntas prováveis:**
- **P: Pra que serve normalizar um vetor?** R: Pra ter só a direção, sem o
  tamanho. Útil quando você quer aplicar uma velocidade fixa em uma direção
  variável.
- **P: Por que operator+ retorna por valor?** R: Não quero modificar nenhum dos
  dois vetores originais, é uma operação pura.

---

### utils/Stats.hpp (Léo)

**O que é:** contadores do jogo e regras de fim de partida.
**Onde está:** `src/utils/Stats.hpp`.
**Quem escreveu:** eu.

**O que eu preciso saber explicar:**
Substituiu o `map<string,int>` que tava no rascunho do Game.cpp original. Map
de string é type-unsafe (qualquer typo vira chave nova silenciosamente),
sem encapsulamento e sem regra de negócio. Com Stats, cada campo é tipado,
cada operação é um método.

`adicionarBanana()` chama `zerarRosa()` por dentro porque é regra do jogo
original: levou banana, perde a streak de rosas. Encapsulei isso aqui pra
Game não ter que lembrar.

`perdeu()` retorna true se banana ou camera chegaram em 3. `venceu()` retorna
true se distancia_metros >= meta_metros - 50 (margem de 50m, igual ao
Python). `operator<<` é friend pra imprimir o HUD direto com `cout`, o
professor permite operadores como exceção da regra de "sem funções livres".

**Perguntas prováveis:**
- **P: Por que substituir o map?** R: Type safety, encapsulamento e pra
  centralizar regras como "banana zera rosa".
- **P: Por que `friend operator<<`?** R: Pra acessar os campos privados sem
  precisar de getters em string e pra usar a sintaxe natural `cout << stats`.

---

### player/Player.hpp + Player.cpp (Thiago/Breno, com `get_rect()` meu cod)

**O que é:** entidade da Gisele com física (gravidade, pulo, posição).
**Onde está:** `src/player/Player.hpp` e `.cpp`.
**Quem escreveu:** Thiago e Breno fizeram a física toda. Eu só adicionei
`get_rect()`.

**O que eu preciso saber explicar:**
A física é simples Euler: `velocidadeY += gravidade * deltaTime`, depois
`y += velocidadeY * deltaTime`. Se passar do chão (`y >= chao`), clampa em
`chao` e zera velocidadeY e o flag `esta_pulando`.

`forca_pulo` é -840 (negativo) porque no sistema de coordenadas deles, Y cresce
pra baixo (igual Pygame). Pular = subir = diminuir Y.

`esta_pulando` impede double jump: `jump()` só aplica força se não estiver
pulando.

`get_rect()` (meu) devolve a hitbox como Rect pro CollisionSystem usar.

**Perguntas prováveis:**
- **P: Por que forca_pulo é negativa?** R: Eixo Y aumenta pra baixo, então
  pular é diminuir Y.
- **P: Como evita double jump?** R: Flag `esta_pulando` vira true quando pula
  e só volta a false quando bate no chão.

---

### collectibles/Collectible.hpp + Collectible.cpp (Thiago/Breno)

**O que é:** classe Base que gera ColetavelData (Rect + sprite_index).
**Onde está:** `src/collectibles/Collectible.hpp` e `.cpp`.
**Quem escreveu:** Thiago e Breno.

**O que eu preciso saber explicar:**
`ColetavelData` é uma struct pequena com a hitbox e o índice do sprite
(0=banana, 1=camera, 2=rosa).

`Base::gerar_coletavel()` escolhe altura entre 3 opções (565, 480, 460),
evitando alturas já ocupadas. Depois sorteia X entre `largura_tela+200` e
`largura_tela+900` (fora da tela à direita) e tenta até 11 vezes achar uma
posição com distância mínima de 250 dos outros coletáveis. Se não achar,
usa a última tentativa mesmo (sem garantia, mas evita loop infinito).

Base é a fábrica. Banana/Camera/Rosa herdam dela mas só pra ter o efeito,
não pra serem fábricas separadas.

**Perguntas prováveis:**
- **P: Por que 11 tentativas?** R: Limite arbitrário pra não ficar infinito.
  Se em 11 sorteios não achou posição boa, aceita a última mesmo.
- **P: Por que altura tem só 3 valores?** R: O jogo original tinha 3 níveis
  de altura (chão, meio, alto) pra player conseguir desviar pulando.

---

### collectibles/Banana.hpp, Camera.hpp, Rosa.hpp (Thiago/Breno, assinaturas atualizadas pelo Léo)

**O que é:** classes de coletáveis com efeito estático ao colidir.
**Onde está:** `src/collectibles/Banana.hpp`, `Camera.hpp`, `Rosa.hpp`.
**Quem escreveu:** Thiago e Breno criaram. Eu mudei a assinatura dos
`efeito_*` pra aceitar `Stats&` no lugar de `map<string,int>&`.

**O que eu preciso saber explicar:**
São header-only com métodos estáticos porque não têm estado próprio. O efeito
é uma função: recebe o Stats, modifica os contadores e sai. Não precisa
instanciar Banana só pra somar 1.

Herdam de Base só pra demonstrar a hierarquia, não precisam usar a factory.
Camera é a que tem estado próprio (flag `flash`, raio, etc) porque o flash
da câmera é uma animação visual.

**Perguntas prováveis:**
- **P: Por que static no efeito?** R: Não tem estado, é uma função pura sobre
  Stats. Não precisa criar instância.
- **P: Por que herdar de Base se não usa a factory?** R: Pra modelar a
  hierarquia "todo coletável é um Base", facilita extensão futura.

---

### systems/CollisionSystem.hpp + CollisionSystem.cpp (Léo)

**O que é:** sistema de detecção e resposta de colisão.
**Onde está:** `src/systems/CollisionSystem.hpp` e `.cpp`.
**Quem escreveu:** eu.

**O que eu preciso saber explicar:**
Classe separada do Game porque colisão é uma responsabilidade própria (Single
Responsibility Principle). Se um dia mudar de AABB pra circle collision, mexo
só aqui.

`verificarColisao()` monta a hitbox do Player com `buildPlayerRect()`, itera
sobre os coletáveis, testa `colliderect`. Se bateu, faz switch no
`sprite_index` e chama o efeito certo (`Banana::efeito_banana(stats)`,
`Camera::efeito_camera(stats)` + liga `flashAtivado`, ou
`Rosa::efeito_rosa(stats)`). Depois marca o coletável como consumido com
sentinela `CONSUMIDO_X = -9999.0f`.

Por que sentinela e não deletar do vector: deletar de um vector enquanto
itera é UB clássico. Marcar com sentinela é seguro e o Spawner reposiciona
na próxima `atualizar()`.

`buildPlayerRect()` é private static porque é helper interno e não usa `this`.

**Perguntas prováveis:**
- **P: Por que não deletar o coletável colidido?** R: Mexer no vector durante
  iteração é problemático. Marco com -9999, o Spawner reposiciona no próximo
  frame.
- **P: Por que CollisionSystem não é método do Game?** R: SRP. Game cuida do
  loop, colisão é assunto separado.

---

### systems/Spawner.hpp + Spawner.cpp (Léo)

**O que é:** gerencia spawn inicial, scroll e respawn dos coletáveis.
**Onde está:** `src/systems/Spawner.hpp` e `.cpp`.
**Quem escreveu:** eu.

**O que eu preciso saber explicar:**
`inicializar()` popula o vector com os 3 coletáveis do começo, cada um
sorteando sprite (rand % 3) e usando `Base::gerar_coletavel` pra não
overlapar com os anteriores.

`atualizar()` faz 3 coisas: (1) move todos pra esquerda em `vel_coletavel`
pixels; (2) pra cada coletável fora da tela (`x + w < 0`) ou consumido
(`x <= -9999`), coleta posições dos OUTROS e gera um novo no lugar;
(3) incrementa o contador de frames e, a cada 300 (10s a 30fps), aumenta
vel_coletavel em 1 até o teto 20. Bate exatamente com o
`pygame.time.set_timer(ACELERAR, 10000)` do Python original.

**Perguntas prováveis:**
- **P: Por que velocidade máxima 20?** R: É o teto do jogo original, mais que
  isso a Gisele não consegue mais reagir.
- **P: Como evita coletáveis em cima um do outro no respawn?** R: Antes de
  gerar, coleto altura e X dos OUTROS coletáveis e passo pra `gerar_coletavel`
  evitar essas posições.

---

### core/GameState.hpp (Léo)

**O que é:** enum de estados possíveis do jogo.
**Onde está:** `src/core/GameState.hpp`.
**Quem escreveu:** eu.

**O que eu preciso saber explicar:**
4 estados: RODANDO, FINALIZANDO, VITORIA, DERROTA.

É `enum class`, não enum tradicional, porque enum class é escopado: pra
referenciar tenho que escrever `GameState::RODANDO`. Evita conflito de nomes
e proíbe conversão implícita pra int.

Transições: RODANDO -> DERROTA (se `stats.perdeu()`), RODANDO -> FINALIZANDO
(se `stats.venceu()`), FINALIZANDO -> VITORIA (quando a Gisele sai da tela).

**Perguntas prováveis:**
- **P: Diferença entre enum e enum class?** R: enum class é escopado, não
  polui o namespace e não converte implicitamente pra int. Mais seguro.

---

### core/Game.hpp + Game.cpp (Léo, reescrito do zero)

**O que é:** loop principal, dona dos subsistemas.
**Onde está:** `src/core/Game.hpp` e `.cpp`.
**Quem escreveu:** eu. O Game.cpp anterior era pseudocódigo quebrado dos
colegas, usei só como referência da lógica e reescrevi inteiro.

**O que eu preciso saber explicar:**
Game é dona por valor de tudo: Player, Camera, CollisionSystem, Spawner,
Stats e o vector de ColetavelData. Sem ponteiros crus, sem `new`/`delete`,
RAII puro.

`renderizar()` é virtual. Isso é Template Method: Game define o passo no
loop, mas delega a implementação real pra subclasse (TerminalRenderer).
Game não precisa saber nada de terminal, ANSI ou termios.

`rodar()` é o loop: processarInput -> atualizar -> checarEstado -> renderizar,
nessa ordem. Input antes de update pra reagir no frame em que o jogador
pressionou. checarEstado depois de update porque update é quem altera o
estado (colisão, distância). Renderizar por último pra mostrar o frame já
atualizado.

`atualizar()` separa o que roda em RODANDO (movimento + colisão + flash) do
que roda em FINALIZANDO (só desliza a Gisele pra fora). `checarEstado()`
ficou separado pra deixar claro qual método faz o quê.

**Perguntas prováveis:**
- **P: Por que renderizar é virtual?** R: Pra TerminalRenderer sobrescrever
  sem Game saber de terminal. É Template Method.
- **P: Por que separar atualizar e checarEstado?** R: SRP. update muda estado,
  checar transiciona pra estado novo.

---

### core/TerminalRenderer.hpp + TerminalRenderer.cpp (Léo)

**O que é:** subclasse de Game que pluga render ASCII e input não-bloqueante.
**Onde está:** `src/core/TerminalRenderer.hpp` e `.cpp`.
**Quem escreveu:** eu.

**O que eu preciso saber explicar:**
Herança e não composição porque TerminalRenderer É UM Game (é uma versão
específica do Game pro terminal), não TEM UM Game. Faz mais sentido na
hierarquia.

Canvas é `vector<string>` 80x20. `plotar(x, y, c)` mapeia coordenada lógica
(0..900, 0..700) pra canvas dividindo proporcionalmente. `flushCanvas()`
manda tudo de uma vez pro stdout, evita flicker.

Em vez de `CLEAR_SCREEN` a cada frame (que pisca), uso `CURSOR_HOME` (`\033[H`)
pra mandar o cursor pro início e sobrescrever em cima do frame anterior.

Termios raw mode: `ICANON` é o modo canônico (terminal só entrega input quando
você aperta Enter). `ECHO` faz a tecla aparecer no terminal. Desativo os dois
pra ler char por char sem ecoar. `VMIN=0` e `VTIME=0` deixam `read()`
não-bloqueante: retorna 0 se não tem nada na fila.

Destructor restaura tudo: `tcsetattr` com termios original e mostra cursor.
Se não restaurar, o terminal do cara fica quebrado (sem echo, modo raw) e
ele tem que rodar `reset` na mão.

**Perguntas prováveis:**
- **P: Herança ou composição?** R: Herança porque TerminalRenderer é um Game
  especializado, não um objeto que tem um Game dentro.
- **P: O que é raw mode?** R: Desativa ICANON (input linha-a-linha) e ECHO
  (mostrar tecla na tela). Permite ler char por char sem o usuário ver o que
  digita.
- **P: Por que CURSOR_HOME no lugar de CLEAR_SCREEN?** R: CLEAR_SCREEN pisca,
  CURSOR_HOME sobrescreve no lugar, ilusão de animação suave.

---

### main.cpp (Léo)

**O que é:** ponto de entrada do programa.
**Onde está:** `src/main.cpp`.
**Quem escreveu:** eu.

**O que eu preciso saber explicar:**
Primeiro imprime a arquitetura (lista as classes de cada pasta). Isso atende
o requisito do professor de "main ilustrar textualmente todo o código".

`srand(time(nullptr))` aqui e não dentro de Game porque srand deve ser
chamado UMA vez por execução. Main é o lugar canônico.

`while (jogar_novamente)` cria um TerminalRenderer novo a cada partida.
Cada nova instância tem estado limpo (Gisele em (100, 525), stats zerados,
coletáveis novos). Saí do replay = saio do while.

**Perguntas prováveis:**
- **P: Por que srand aqui e não no Game?** R: srand é estado global, só
  deve rodar uma vez por execução.

---

## Perguntas Prováveis do Professor

**P: Por que usar herança entre TerminalRenderer e Game?**
R: Porque TerminalRenderer É UMA versão do Game (especialização pra terminal).
Permite sobrescrever `renderizar()` e `processarInput()` sem mexer na lógica
da classe base. Padrão Template Method.

**P: O que é AABB e como você implementou?**
R: Axis-Aligned Bounding Box. Dois retângulos alinhados aos eixos colidem se,
em CADA eixo, eles se sobrepõem. Implementei em `Rect::colliderect`: quatro
comparações de borda em uma expressão única.

**P: Por que CollisionSystem é uma classe separada?**
R: Single Responsibility Principle. Game cuida do loop, colisão é assunto
isolado. Se um dia mudar pra circle collision, mexo só nessa classe.

**P: O que é enum class e qual a diferença pra enum tradicional?**
R: enum class é escopado: pra acessar tenho que escrever `GameState::RODANDO`.
Evita poluição de namespace e proíbe conversão implícita pra int. Mais seguro.

**P: Por que você usou virtual?**
R: No `renderizar()` e `processarInput()` do Game. Permite a subclasse
sobrescrever sem a base saber. É como o loop chama a versão certa em runtime
(polimorfismo dinâmico).

**P: O que são métodos static e quando usar?**
R: Método que não usa `this`, pertence à classe não à instância. Usei nos
`efeito_*` das Banana/Camera/Rosa porque é função pura sobre Stats, não
precisa de instância.

**P: Por que Stats substituiu o map<string,int>?**
R: Map de string é type-unsafe (typo vira chave nova silenciosamente), sem
encapsulamento e sem regra de negócio. Stats deixa cada campo tipado e
permite encapsular regras como "banana zera rosa" num só método.

**P: O que é o sentinel -9999 no coletável consumido?**
R: Valor mágico que marca "esse coletável já foi consumido nesse frame".
Não dá pra deletar do vector durante iteração (UB), então marco e o Spawner
reposiciona depois.

**P: Como funciona o raw mode do terminal?**
R: Desativo ICANON (input linha-a-linha) e ECHO (mostrar tecla). VMIN=0 e
VTIME=0 fazem `read()` retornar imediatamente, com ou sem dados. Restauro
no destrutor pra não quebrar o terminal do usuário.

**P: Como você garantiu que compila no Windows?**
R: Só C++17 e stdlib, zero biblioteca externa. ANSI escape codes funcionam
no Windows 10+ nativamente. Termios funciona via MSYS2/MinGW. Recomendo
WSL2 pra melhor experiência.

**P: O que é Single Responsibility Principle e onde aparece no código?**
R: Cada classe faz uma coisa só. Rect faz hitbox, Stats faz contadores,
Spawner faz spawn, CollisionSystem faz colisão. Game só orquestra.

**P: Por que o destructor do TerminalRenderer é importante?**
R: Restaura o termios original e mostra o cursor de novo. Sem isso, se o
jogo crashar ou o cara apertar Ctrl+C, o terminal fica quebrado.

**P: O que é friend function e onde você usou?**
R: Função externa que pode acessar membros privados da classe. Usei em
`operator<<` de Rect, Vector2 e Stats pra poder imprimir com `cout`. O
professor permite operadores como exceção da regra "sem funções livres".

**P: Como funciona a física do pulo?**
R: Integração de Euler simples. velocidadeY acumula `gravidade * deltaTime`,
y acumula `velocidadeY * deltaTime`. Quando passa do chão, clampa e zera
velocidade. Pulo aplica força negativa instantânea em velocidadeY.

**P: Por que o loop de spawn tenta 11 vezes pra posicionar o coletável?**
R: Limite pra não ficar infinito. Se em 11 sorteios não achou X com distância
mínima 250 dos outros, aceita a última tentativa mesmo. Trade-off entre
qualidade do spawn e garantia de terminar.

**P: Por que Game guarda subsistemas por valor e não por ponteiro?**
R: RAII puro. Não precisa de `new`/`delete`, vida útil é a do Game, zero
risco de memory leak. C++ moderno prefere composição por valor sempre que
possível.

**P: Por que `explicit` no construtor de Spawner e TerminalRenderer?**
R: Evita conversão implícita acidental. `TerminalRenderer t = 900;` ia
compilar sem explicit, com explicit dá erro.

**P: Como você passa o jogo de RODANDO pra FINALIZANDO pra VITORIA?**
R: `checarEstado()` roda todo frame. Se stats.venceu(), vira FINALIZANDO. Em
FINALIZANDO, a Gisele desliza pra fora da tela; quando `x >= largura_tela`,
vira VITORIA e o loop termina.

---

## O que Foi Feito por Cada Integrante

**Thiago e Breno (antes da minha branch):**
- Player.hpp/cpp (física completa: gravidade, pulo, clamp no chão)
- Collectible.hpp/cpp (Base com `gerar_coletavel`)
- Banana.hpp, Camera.hpp, Rosa.hpp (efeitos originais com map<string,int>)
- Estrutura de pastas inicial e README inicial
- Game.cpp original (pseudocódigo que serviu de referência)

**Léo (esta branch):**
- utils/Rect.hpp (classe com AABB)
- utils/Vector2.hpp (classe com operadores)
- utils/Stats.hpp (substitui o map, encapsula contadores e regras)
- systems/CollisionSystem.hpp/cpp (detecção AABB e resposta)
- systems/Spawner.hpp/cpp (spawn, scroll, respawn, aceleração)
- core/GameState.hpp (enum class)
- core/Game.hpp/cpp (reescrita total do pseudocódigo)
- core/TerminalRenderer.hpp/cpp (render ASCII, input, telas)
- src/main.cpp (demo da arquitetura + loop de replay)
- Player.hpp/cpp: adicionei `get_rect()`
- Banana/Camera/Rosa: troquei assinatura pra `Stats&`
- README.md: reescrevi com instruções reais
- .gitignore
- Este guia de estudo

---

## Dicas de Apresentação

1. **Falar a lógica em voz alta antes de mostrar o código.** "O loop pega
   input, atualiza, checa estado, renderiza, e isso roda 30 vezes por
   segundo." Depois abro o arquivo.

2. **Se não lembrar um detalhe, abrir o arquivo e ler junto com o professor.**
   Não tem problema, mostra que sei navegar no projeto.

3. **Não decorar código, entender fluxo.** O fluxo é: main -> Game::rodar ->
   loop (input/update/check/render) -> efeito em Stats -> render lê estado.

4. **Se perguntarem sobre código do Thiago ou Breno, falar com naturalidade
   "esse foi o Thiago/Breno, mas eu entendo assim..." e explicar.** O
   professor não cobra autoria, cobra entendimento.

5. **Pontos que eu DOMINO porque são meus:** CollisionSystem, Stats, Spawner,
   Game.cpp, TerminalRenderer, main. Qualquer pergunta nessas áreas eu mando
   ver.

6. **Pontos que preciso revisar mais (não são meus):** física do Player e o
   loop de tentativas em Collectible.cpp. Reler antes da apresentação.

7. **Se travar, lembrar: arquitetura em camadas, separação de
   responsabilidades, herança virtual no render.** Esses três conceitos
   cobrem 80% das perguntas de OOP.
