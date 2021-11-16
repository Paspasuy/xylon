#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include "SongSearch.h"

SongSearch::SongSearch(Player *_p) {
    p = _p;
}

Player *SongSearch::rebuild() {
    if (text.empty()) {
        return p;
    }
    std::vector<Song *>::iterator iter = p->songs.begin();
    std::vector<std::vector<Song *>::iterator> v;
    int idx = -1;
    int id = p->get_current_id();
    while (iter != p->songs.end()) {
        if ((*iter)->matches(text)) {
            if (id == (*iter)->id) {
                idx = v.size();
            }
            v.push_back(iter);
        }
        ++iter;
    }
    if (v.empty()) {
        back_col = sf::Color(128, 32, 32);
        if (p2 == nullptr)
            return p;
        return p2;
    }
    back_col = sf::Color(32, 32, 32);
    if (p2 != nullptr)
        delete p2;
    p2 = new Player(p);
    p2->ptr = idx;
    for (auto it: v) {
        p2->songs.push_back(*it);
    }
    return p2;
}

Player *SongSearch::add_char(wchar_t c) {
    if (size(text) == 0) {}
    std::locale loc = std::wcin.getloc();
    text += std::use_facet<std::ctype<wchar_t>>(loc).tolower(c);
    return rebuild();
}

Player *SongSearch::pop_char() {
    if (text.size() != 0) {
        text.pop_back();
    }
    return rebuild();
}

Player *SongSearch::pop_word() {
    if (text.size() && text.back() == ' ') {
        text.pop_back();
    }
    while (text.size() != 0 && text.back() != ' ') {
        text.pop_back();
    }
    return rebuild();
}

Player *SongSearch::clear() {
    text.clear();
    delete p2;
    p2 = nullptr;
    return p;
}

void SongSearch::render(sf::RenderWindow &window, sf::Font &font) {
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

bool SongSearch::state() {
    return !text.empty();
}
