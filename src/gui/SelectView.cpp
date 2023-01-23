#include "SelectView.h"
#include "../Tile.h"
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Text.hpp>

SelectView::SelectView(std::initializer_list<std::string> lst) : items(lst) {}

SelectView::~SelectView() {}

void SelectView::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    if (!show) return;
    sf::RectangleShape dim(sf::Vector2f(target.getSize()));
    dim.setFillColor(sf::Color(0, 0, 0, 180));
    target.draw(dim);
    int cursh = std::max(10, std::min((int)target.getSize().y - 2 * H, 10 + ptr * H));
    int sh = cursh - ptr * H;
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
        sh += H;
        ++idx;
    }
}

void SelectView::up() {
    --ptr;
    ptr = std::max(0, ptr);
}

void SelectView::down() {
    ++ptr;
    ptr = std::min((int)items.size() - 1, ptr);
}

SelectView::SelectView() {}

void SelectView::pageUp() {
    ptr -= PAGE;
    ptr = std::max(0, ptr);
}

void SelectView::pageDown() {
    ptr += PAGE;
    ptr = std::min((int)items.size() - 1, ptr);
}
