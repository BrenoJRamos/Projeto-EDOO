#include "Player.hpp"

// Construtor
Player::Player() {

    //posição inicial da personagem
    x = 120.0f;
    y = 525.0f;

    //tamanho da hitbox
    largura = 50.0f;
    altura = 70.0f;

    //velocidade
    velocidadeX = 8.0f;
    velocidadeY = 0.0f;

    //gravidade
    gravidade = 60.0f;

    //força do pulo
    forca_pulo = -840.0f;

    //estados iniciais
    esta_pulando = false;
    esta_andando = true;

    //posição do chão
    chao = 525.0f;
}

// pulo
void Player::jump() {

    // impede pulo se já estiver pulando
    if (!esta_pulando) {
        velocidadeY = forca_pulo;
        esta_pulando = true;
    }
}

// atualização da física
void Player::update_fisica(float deltaTime) {

    // aplicação da gravidade
    velocidadeY += gravidade * deltaTime;

    // mover personagem verticalmente
    y += velocidadeY * deltaTime;

    // colisão 
    if (y >= chao) {
        y = chao;
        velocidadeY = 0.0f;
        esta_pulando = false;
    }

}

// getters
float Player::get_X() const {
    return x;
}

float Player::get_Y() const {
    return y;
}

float Player::get_largura() const {
    return largura;
}

float Player::get_altura() const {
    return altura;
}

bool Player::get_esta_pulando() const {
    return esta_pulando;
}

float Player::get_velocidadeX() const {
    return velocidadeX;
}

bool Player::get_esta_andando() const {
    return esta_andando;
}

// setters
void Player::setPosition(float newX, float newY) {
    x = newX;
    y = newY;
}

void Player::setWalking(bool walking) {
    esta_andando = walking;
}

void Player::setJumping(bool jumping) {
    esta_andando = jumping;
}

void Player::setX(int novox){
    x = novox;
}

void Player::setY(int novoy){
    y = novoy;
}
