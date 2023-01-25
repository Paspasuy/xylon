#include "SelectView.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Text.hpp>
#include <codecvt>

#include "../Tile.h"

SelectView::SelectView(std::initializer_list<std::string> lst) : items(lst), visibleItems(lst) {}

SelectView::~SelectView() {}

void SelectView::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    if (!show) return;
    sf::RectangleShape dim(sf::Vector2f(target.getSize()));
    dim.setFillColor(sf::Color(0, 0, 0, 180));
    target.draw(dim);
    int sh = shift = clamp(shift, 10 - ptr * H, target.getSize().y - 30 - ptr * H);
    int idx = 0;
    for (auto& s : visibleItems) {
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
    ptr = std::min((int)visibleItems.size() - 1, ptr);
}

SelectView::SelectView() {}

void SelectView::pageUp() {
    ptr -= PAGE;
    ptr = std::max(0, ptr);
}

void SelectView::pageDown() {
    ptr += PAGE;
    ptr = std::min((int)visibleItems.size() - 1, ptr);
}

void SelectView::filter(const std::string& str) {
    ptr = 0;
    if (str == "") {
        visibleItems = items;
    } else {
        visibleItems.clear();
        for (std::string& item: items) {
            if (hasSubstr(item, str)) {
                visibleItems.emplace_back(item);
            }
        }
    }
}

void SelectView::filter(const std::wstring& str) {
    using convert_type = std::codecvt_utf8<wchar_t>;
    std::wstring_convert<convert_type, wchar_t> converter;
    filter(converter.to_bytes(str));
}

size_t SelectView::size() { return visibleItems.size(); }
