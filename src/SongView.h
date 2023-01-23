#ifndef XYLON_SONGVIEW_H
#define XYLON_SONGVIEW_H

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "Player.h"
#include "Tile.h"
#include "gui/ProgressBarView.h"
#include "utils/Settings.h"
#include "utils/Utils.h"

class SongView {
    std::vector<Tile> tiles;
    sf::Time t;
    int shift = 0;
    int vel = 0;
    int cur = -1;
    const int accel = 1500;
    const int FAST = 2000;
    const int TILE_GAP = 2;
    const int MAX_VEL = 3000;
    const int MAX_VEL_WITH_SHIFT = 30000;
    int last_y = 0;
    Player* pl;

    int get_low_tile();

    int get_up_tile(int winh);

    const int CUR_SHIFT = 50;

public:
    bool holding = false;
    bool shift_pressed = false;

    sf::Vector2<unsigned int> winsz;

    void render(sf::RenderWindow& window, PicLoader& pl);

    void init(Player* p, const std::wstring& filter = L"");

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

    size_t size();

    void play_next();

    void play_prev();
};

#endif  // XYLON_SONGVIEW_H
