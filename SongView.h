#ifndef XYLON_SONGVIEW_H
#define XYLON_SONGVIEW_H

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "Tile.h"
#include "Player.h"
#include "ProgressBarView.h"

class SongView {
    std::vector<Tile *> tiles;
    int shift = 0;
    Player *pl;

    int get_low_tile();

    int get_up_tile(int winh);

    const int CUR_SHIFT = 50;
public:
    sf::Vector2<unsigned int> winsz;

    void render(sf::RenderWindow &window, sf::Font &font, sf::Font &bold_font);

    void init(Player *p);

    void norm_shift();
    void norm_shift_up();

    void norm_shift_down();

    void scroll(int delta);

    int get_click_id(int x, int y);
};


#endif //XYLON_SONGVIEW_H
