# 🎮 Projeto de EDOO  
## Conversão do jogo **“Gisele Bündchen vs As Forças do Mal”** de Python para C++

Este repositório contém o projeto desenvolvido para a disciplina **Estrutura de Dados Orientada a Objetos (EDOO)**.  
O objetivo principal é **converter e reestruturar** um jogo originalmente criado em **Python**, implementando-o agora em **C++**, com foco em modularização, orientação a objetos e boas práticas de arquitetura.

---

## 📘 Sobre o Projeto

O jogo escolhido pelo grupo foi:

> **Gisele Bündchen vs As Forças do Mal**  
> Um jogo divertido originalmente criado como atividade da disciplina de Introdução à Programação.  
> Nesta versão, o jogo foi totalmente reescrito em C++ com estruturas mais robustas, aplicando princípios de Programação Orientada a Objetos.

---

## 🧱 Estrutura do Projeto

A organização do diretório `src/` segue um padrão modular dividido por responsabilidade:

```text
src/
│
├── core/
│ ├── Game.cpp
│ ├── Game.hpp
│ ├── GameState.hpp
│
├── player/
│ ├── Player.cpp
│ ├── Player.hpp
│
├── collectibles/
│ ├── Collectible.cpp
│ ├── Collectible.hpp
│ ├── Banana.cpp
│ ├── Camera.cpp
│ ├── Rose.cpp
│
├── systems/
│ ├── CollisionSystem.cpp
│ ├── CollisionSystem.hpp
│ ├── Spawner.cpp
│ ├── Spawner.hpp
│
├── utils/
│ ├── Rect.hpp
│ ├── Vector2.hpp
│ ├── Stats.hpp
│
└── main.cpp
```

---

## 🛠️ Tecnologias Utilizadas

- **C++17**
- **Programação Orientada a Objetos**
- Estruturas auxiliares próprias:
  - `Vector2` (vetores 2D)
  - `Rect` (colisões e áreas)
  - `Stats` (atributos e status)
- Sistemas independentes para:
  - Colisão (`CollisionSystem`)
  - Spawn de itens (`Spawner`)
- Arquitetura modular baseada em entidades e sistemas

---

## 🚀 Como compilar e executar o projeto

*(Seção ainda será preenchida futuramente.)*

---

## 🎓 Disciplina

Este projeto foi realizado para a disciplina **Estrutura de Dados Orientada a Objetos (EDOO)**, aplicando conceitos como:

- Abstração  
- Encapsulamento  
- Herança e polimorfismo  
- Modularização  
- Organização e estruturação de projetos em C++

---

## 👥 Integrantes

- Thiago José Barbosa Menezes de Oliveira (tjbmo)
- Breno José Ramos da Silva (bjrs)
- Leonardo Gonçalves Sobral (lgs5)


---

## 📄 Licença

Este projeto é de caráter acadêmico e possui fins exclusivamente educacionais.

---
