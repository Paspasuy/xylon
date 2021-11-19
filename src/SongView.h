#ifndef XYLON_SONGVIEW_H
#define XYLON_SONGVIEW_H

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "Tile.h"
#include "Player.h"
#include "ProgressBarView.h"

class SongView {
    std::vector<Tile *> tiles;
    sf::Time t;
    int shift = 0;
    int vel = 0;
    const int accel = 1500;
    const int MAX_VEL = 3000;
    int last_y = 0;
    Player *pl;

    int get_low_tile();

    int get_up_tile(int winh);

    const int CUR_SHIFT = 50;
public:
    bool holding = false;
    sf::Vector2<unsigned int> winsz;

    void render(sf::RenderWindow &window, sf::Font &font, sf::Font &bold_font, sf::Time time);

    void init(Player *p);

    void norm_shift();

    void norm_shift_up();

    void norm_shift_down();

    void scroll(int delta);

    void update_shift(sf::Time t);

    void update_vel(sf::Time t);

    std::pair<int, int> get_click_id(int x, int y);

    void grab(int y);

    void release(int y, sf::Time time);

    void set_position(int y);

    void norm_shift_tile();

    void pageup();

    void pagedown();
};


#endif //XYLON_SONGVIEW_H
