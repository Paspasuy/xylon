#include "Visualiser.h"

#include <SFML/Graphics/RectangleShape.hpp>
#include <cmath>
#include <iostream>

#include "../Tile.h"

Visualiser::~Visualiser() {}

void Visualiser::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    if (!display) return;
    int len = target.getSize().x - Tile::W;
    sf::RectangleShape sh;
    const int w = 4;
    const int SPACE = 1;
    float down_c = 1.5;
    sh.setFillColor(settings.c6);
    sh.setOutlineColor(settings.c6);
    int h;
    if (settings.vis_type == 0) {
        h = target.getSize().y;
    } else if (settings.vis_type == 1) {
        h = target.getSize().y * 3 / 4;
    }
    for (int i = 1; i < len / (w + SPACE); i += 1) {
        fft[i] /= down_c;
//        fft[i] = std::abs(fft[i]);
//        float mid = fft[i];
        float mid = fft[i - 1] * 0.25 + fft[i] * 0.5 + fft[i + 1] * 0.25;
//        float mid = (fft[i - 2] + fft[i - 1] + fft[i] + fft[i + 1] + fft[i + 2]) / 5;
        sh.setSize(sf::Vector2f(w, std::min(1000 * mid, 1500.f)));
        if (settings.vis_type == 0) {
            sh.setPosition(i * (w + SPACE), h - sh.getSize().y);
        } else if (settings.vis_type == 1) {
            sh.setPosition(i * (w + SPACE), h - sh.getSize().y / 2);
        }
        target.draw(sh);
    }
}
