#pragma once

// Classe do player

class Player {
private:

    // Posição da Gisele
    float x;
    float y;

    // Tamanho da hitbox
    float largura;
    float altura;

    // Física
    float velocidadeX;   // velocidade horizontal
    float velocidadeY;   // velocidade vertical
    float gravidade;     // força da gravidade
    float forca_pulo;   // força do pulo

    // Estados
    bool esta_pulando;    // sinaliza se está pulando
    bool esta_andando;    // sinaliza se está andando 

    // Chão
    float chao;

public:

    // Construtor
    Player();

    // funções principais
    void jump();
    void update_fisica(float deltaTime);

    // getters
    float get_X() const;
    float get_Y() const;
    float get_largura() const;
    float get_altura() const;
    float get_velocidadeX() const;
    bool get_esta_pulando() const;
    bool get_esta_andando() const;

    // Setters
    void setPosition(float newX, float newY);
    void setWalking(bool walking);
};
