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
    int len = window.getSize().x - Tile::W;
    sf::RectangleShape sh;
    int w = 3;
    float down_c = 1.5;
    sh.setFillColor(sf::Color::Magenta);
    for (int i = 1; i < len / w; i += 1) {
        fft[i] /= down_c;
        sh.setSize(sf::Vector2f(w, std::min(700  * std::sqrt(fft[i]), 1500.f)));
        sh.setPosition(i * w, window.getSize().y - sh.getSize().y);
        window.draw(sh);
    }
}
