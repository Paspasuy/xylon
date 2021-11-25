//
// Created by pavel on 11/25/21.
//

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include "SortSelect.h"

void SortSelect::up() {
    ptr += sorts.size() - 1;
    ptr %= sorts.size();
}

void SortSelect::down() {
    ++ptr;
    ptr %= sorts.size();
}

void SortSelect::render(sf::RenderWindow &window, sf::Font font) {
    if (!state)
        return;
    sf::RectangleShape dim(sf::Vector2f(window.getSize()));
    dim.setFillColor(sf::Color(0, 0, 0, 180));
    window.draw(dim);
    int sh = 10;
    int idx = 0;
    for (auto &s: sorts) {
        sf::Text text(s, font, 20);
        text.setPosition(10, sh);
        if (idx == ptr) {
            text.setFillColor(sf::Color::Blue);
        } else {
            text.setFillColor(sf::Color::White);
        }
        window.draw(text);
        sh += 30;
        ++idx;
    }
}

void SortSelect::sort(Player *p) {
    if (ptr == 0) {
        p->sort_by_date();
    } else if (ptr == 1) {
        p->sort_by_album();
    }
}
