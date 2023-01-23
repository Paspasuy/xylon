#include "Visualiser.h"

#include <SFML/Graphics/RectangleShape.hpp>
#include <cmath>
#include <iostream>

#include "../Tile.h"

Visualiser::~Visualiser() {}

void Visualiser::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    if (!display) return;
    int len = target.getSize().x - Tile::W;
    sf::RectangleShape sh;
    int w = 4;
    float down_c = 1.5;
    sh.setFillColor(settings.c6);
    int h;
    if (settings.vis_type == 0) {
        h = target.getSize().y;
    } else if (settings.vis_type == 1) {
        h = target.getSize().y * 3 / 4;
    }
    for (int i = 1; i < len / (w + 1); i += 1) {
        fft[i] /= down_c;
        sh.setSize(sf::Vector2f(w, std::min(700 * std::sqrt(fft[i]), 1500.f)));
        if (settings.vis_type == 0) {
            sh.setPosition(i * (w + 1), h - sh.getSize().y);
        } else if (settings.vis_type == 1) {
            sh.setPosition(i * (w + 1), h - sh.getSize().y / 2);
        }
        target.draw(sh);
    }
}
