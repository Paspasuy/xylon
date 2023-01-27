#ifndef XYLON_TILE_H
#define XYLON_TILE_H

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>

#include "Song.h"
#include "utils/PicLoader.h"

class Tile {
    sf::Text title_text;
    sf::Text artist_text;
    sf::Text time_text;
    sf::Text album_text;
public:
    static int MAX_W;
    static int W;
    static const int H = 80;
    const int IMG_ALPHA = 100;
    const int FADE_TIME = 300;
    sf::Vector2i position;
    Song* s;
    bool pic_transparent = false;

    explicit Tile(Song* _s);

    void render(sf::RenderTarget& target, PicLoader& pl, sf::RectangleShape& sh, int elapsed,
                bool too_fast, bool is_cur);
};

#endif  // XYLON_TILE_H
