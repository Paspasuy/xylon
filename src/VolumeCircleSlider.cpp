//
// Created by pavel on 11/15/21.
//

#include "VolumeCircleSlider.h"

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Text.hpp>

#include "Tile.h"

VolumeCircleSlider::VolumeCircleSlider(Player* _p, sf::Time _t) : shape(VolumeShape(40.f)) {
    t = _t - sf::seconds(2);
    p = _p;
    shape.setFillColor(sf::Color::Transparent);
    shape.setOutlineThickness(7.f);
}

void VolumeCircleSlider::render(sf::RenderWindow& window, sf::Font& bold_font, sf::Time _t) {
    float time = (_t - t).asSeconds();
    if (time < 1.25) {
        float alpha = std::min(1., 4. * (1.25 - time));
        int val = int(100 * (p->vol + 0.001));
        sf::Text txt(std::to_string(val) + "%", bold_font, 20);
        sf::Color cur_color = sf::Color(255, 255, 255, int(alpha * 255));
        txt.setFillColor(cur_color);
        int x = 70;  // window.getSize().x - Tile::W - 70;
        int y = window.getSize().y - 90;
        txt.setPosition(x - txt.getLocalBounds().width / 2, y - txt.getLocalBounds().height);
        window.draw(txt);
        shape.touch(val);
        shape.setOutlineColor(cur_color);
        shape.setPosition(x, y);
        window.draw(shape);
    }
}

void VolumeCircleSlider::touch(sf::Time _t) { t = _t; }
