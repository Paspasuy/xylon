//
// Created by pavel on 11/19/21.
//

#include <cmath>
#include <iostream>
#include <SFML/Graphics/RectangleShape.hpp>
#include "Visualiser.h"
#include "Tile.h"

Visualiser::Visualiser() {

}

void Visualiser::render(sf::RenderWindow &window, float *fft) {
    if (!display)
        return;
    int len = window.getSize().x - Tile::W;
    sf::RectangleShape sh;
    int w = 4;
    float down_c = 1.5;
    sh.setFillColor(sf::Color::Magenta);
//    int h = window.getSize().y * 3 / 4;
    int h = window.getSize().y;
    for (int i = 1; i < len / (w + 1); i += 1) {
        fft[i] /= down_c;
        sh.setSize(sf::Vector2f(w, std::min(700  * std::sqrt(fft[i]), 1500.f)));
//        sh.setPosition(i * (w + 1), h - sh.getSize().y / 2);
        sh.setPosition(i * (w + 1), h - sh.getSize().y);
        window.draw(sh);
    }
}
