#ifndef RECT_HPP
#define RECT_HPP

#include <iostream>

// retangulo de hitbox, uso float pra bater com o resto que ja ta em float
class Rect {
private:
    float x, y, w, h;

public:
    Rect(float x = 0, float y = 0, float w = 0, float h = 0)
        : x(x), y(y), w(w), h(h) {}

    // AABB padrao, compara as quatro bordas
    bool colliderect(const Rect& other) const {
        return x < other.x + other.w &&
               x + w > other.x &&
               y < other.y + other.h &&
               y + h > other.y;
    }

    float getX() const { return x; }
    float getY() const { return y; }
    float getW() const { return w; }
    float getH() const { return h; }
    float getRight() const { return x + w; }
    float getBottom() const { return y + h; }

    void setX(float nx) { x = nx; }
    void setY(float ny) { y = ny; }
    void setW(float nw) { w = nw; }
    void setH(float nh) { h = nh; }

    friend std::ostream& operator<<(std::ostream& os, const Rect& r) {
        os << "Rect(" << r.x << ", " << r.y << ", " << r.w << "x" << r.h << ")";
        return os;
    }
};

#endif
