#ifndef XYLON_TILE_H
#define XYLON_TILE_H

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include "Song.h"


class Tile {
public:
    static int MAX_W;
    static int W;
    static const int H = 80;
    sf::Vector2i position;
    Song *s;
    int margin = 0;

    Tile(Song *_s);

    void render(sf::RenderWindow &window, sf::Font &font, sf::Font &bold_font, sf::RectangleShape &sh, bool is_cur);
};

#endif //XYLON_TILE_H
