#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include "SongSearch.h"

void SongSearch::add_char(wchar_t c) {
    if (size(text) == 0) {}
    std::locale loc = std::wcin.getloc();
    text += std::use_facet<std::ctype<wchar_t>>(loc).tolower(c);
}

void SongSearch::pop_char() {
    if (text.size() != 0) {
        text.pop_back();
    }
}

void SongSearch::pop_word() {
    while (text.size() && text.back() == ' ') {
        text.pop_back();
    }
    while (text.size() != 0 && text.back() != ' ') {
        text.pop_back();
    }
}

void SongSearch::clear() {
    text.clear();
}

void SongSearch::render(sf::RenderWindow& window, sf::Font &font) {
    if (!text.empty()) {
        sf::Text txt(L"search: " + text, font, 20);
        sf::RectangleShape back(sf::Vector2f((text.size() + 8) * 12 + 4, 26));
        txt.setPosition(0, window.getSize().y - 30);
        back.setPosition(txt.getPosition());
        back.setFillColor(back_col);
        window.draw(back);
        window.draw(txt);
    }
}

bool SongSearch::empty() {
    return text.empty();
}

const std::wstring& SongSearch::get_filter() {
    return text;
}

void SongSearch::update_color(size_t len) {
    if (len) {
        back_col = sf::Color(32, 32, 32);
    } else {
        back_col = sf::Color(128, 32, 32);
    }
}
