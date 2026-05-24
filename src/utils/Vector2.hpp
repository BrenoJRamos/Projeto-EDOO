#ifndef VECTOR2_HPP
#define VECTOR2_HPP

#include <iostream>
#include <cmath>

// vetor 2D utilitario, nao uso muito no jogo (player ja trabalha com float cru),
// mas mantenho aqui porque a arquitetura prevê e fica disponivel pra qualquer
// extensao futura
class Vector2 {
private:
    float x, y;

public:
    Vector2(float x = 0, float y = 0) : x(x), y(y) {}

    Vector2 operator+(const Vector2& o) const { return Vector2(x + o.x, y + o.y); }
    Vector2 operator-(const Vector2& o) const { return Vector2(x - o.x, y - o.y); }
    Vector2 operator*(float s) const { return Vector2(x * s, y * s); }

    Vector2& operator+=(const Vector2& o) { x += o.x; y += o.y; return *this; }
    Vector2& operator-=(const Vector2& o) { x -= o.x; y -= o.y; return *this; }

    bool operator==(const Vector2& o) const { return x == o.x && y == o.y; }

    float magnitude() const { return std::sqrt(x * x + y * y); }

    // se for vetor zero devolve zero mesmo, evita divisao por zero
    Vector2 normalize() const {
        float m = magnitude();
        if (m == 0.0f) return Vector2(0, 0);
        return Vector2(x / m, y / m);
    }

    float dot(const Vector2& o) const { return x * o.x + y * o.y; }

    float getX() const { return x; }
    float getY() const { return y; }
    void setX(float nx) { x = nx; }
    void setY(float ny) { y = ny; }

    friend std::ostream& operator<<(std::ostream& os, const Vector2& v) {
        os << "(" << v.x << ", " << v.y << ")";
        return os;
    }
};

#endif
