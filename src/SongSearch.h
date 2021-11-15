#ifndef XYLON_SONGSEARCH_H
#define XYLON_SONGSEARCH_H

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Font.hpp>
#include "Player.h"

class SongSearch {
public:
    Player *p, *p2 = nullptr;
    std::wstring text;
    sf::Color back_col = sf::Color(32, 32, 32);

    SongSearch(Player *_p);

    Player *rebuild();

    Player *add_char(wchar_t c);

    Player *pop_char();

    Player *pop_word();

    Player *clear();

    void render(sf::RenderWindow &window, sf::Font &font);

    bool state();
};

#endif //XYLON_SONGSEARCH_H
