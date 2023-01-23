#ifndef XYLON_TILE_H
#define XYLON_TILE_H

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include "PicLoader.h"
#include "Song.h"

class Tile {
public:
    static int MAX_W;
    static int W;
    static const int H = 80;
    const int IMG_ALPHA = 100;
    const int FADE_TIME = 300;
    sf::Vector2i position;
    Song* s;
    bool pic_transparent = false;

    Tile(Song* _s);

    void render(sf::RenderWindow& window, PicLoader& pl, sf::RectangleShape& sh, bool too_fast,
                bool is_cur);
};

#endif  // XYLON_TILE_H
