#ifndef XYLON_SONGSEARCH_H
#define XYLON_SONGSEARCH_H

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include "Player.h"

class SongSearch {
public:
    std::wstring text;
    sf::Color back_col = sf::Color(32, 32, 32);

    void add_char(wchar_t c);

    void pop_char();

    void pop_word();

    void clear();

    void render(sf::RenderWindow& window, sf::Font& font);

    bool empty();

    const std::wstring& get_filter();

    void update_color(size_t i);
};

#endif  // XYLON_SONGSEARCH_H
