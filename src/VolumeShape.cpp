#include "VolumeShape.h"

#include <iostream>

VolumeShape::VolumeShape(float radius) : r(radius) {
    setFillColor(sf::Color::Transparent);
    update();
}

void VolumeShape::setRadius(float radius) {
    r = radius;
    update();
}

const float& VolumeShape::getRadius() const { return r; }

sf::Vector2f VolumeShape::getPoint(std::size_t index) const {
    int ind = index;
    int cnt = getPointCount() / 2;
    if (index >= cnt) {
        ind = getPointCount() - ind - 1;
    }
    static const float pi = 3.141592654f;
    float angle = (ind * 2 * pi / (cnt - 1) - pi) * prog * 0.01 + pi / 2;
    float x = std::cos(angle) * r;
    float y = std::sin(angle) * r;
    return sf::Vector2f(x, y);
}

void VolumeShape::touch(int val) {
    if (val != prog) {
        prog = val;
        update();
    }
}

std::size_t VolumeShape::getPointCount() const { return 300; }
