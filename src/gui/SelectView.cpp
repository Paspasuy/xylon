#include "SelectView.h"
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Text.hpp>

SelectView::SelectView(std::initializer_list<std::string> lst) : items(lst) {}

SelectView::~SelectView() {}

void SelectView::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    if (!show) return;
    sf::RectangleShape dim(sf::Vector2f(target.getSize()));
    dim.setFillColor(sf::Color(0, 0, 0, 180));
    target.draw(dim);
    int sh = 10;
    int idx = 0;
    for (auto& s : items) {
        sf::Text text(s, FONT, 20);
        text.setPosition(10, sh);
        if (idx == ptr) {
            text.setFillColor(sf::Color::Blue);
        } else {
            text.setFillColor(sf::Color::White);
        }
        target.draw(text);
        sh += 30;
        ++idx;
    }
}

void SelectView::up() {
    ptr += items.size() - 1;
    ptr %= items.size();
}

void SelectView::down() {
    ++ptr;
    ptr %= items.size();
}
