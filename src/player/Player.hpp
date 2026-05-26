#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <Collectible.hpp> // para o tipo Rect (usado em get_rect)
#include <ostream>         // para o operator<< (impressao com cout)

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
    float velocidadeX;   
    float velocidadeY; 
    float gravidade;     
    float forca_pulo; 

    // Estados
    bool esta_pulando;
    bool esta_andando;    

    // Chão
    float chao;

public:

    //Construtor
    Player();

    //funções principais
    void jump();
    void update_fisica(float deltaTime = 1.0f/30.0f);

    //hitbox da personagem (equivale ao gisele.get_rect() do Python)
    Rect get_rect() const;

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
    void setJumping(bool jumping);
    void setX(int novox);
    void setY(int novoy);
};

inline std::ostream& operator<<(std::ostream& os, const Player& p){
    os << "posicao inicial: x=" << p.get_X() << " y=" << p.get_Y() << "\n";
    os << "hitbox: largura=" << p.get_largura() << " altura=" << p.get_altura() << "\n";
    os << "velocidade horizontal: " << p.get_velocidadeX() << "\n";
    os << "andando: " << (p.get_esta_andando() ? "sim" : "nao");
    return os;
}

#endif