#ifndef XYLON_SONGLIST_H
#define XYLON_SONGLIST_H

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "Player.h"
#include "Tile.h"
#include "gui/ProgressBarView.h"
#include "utils/Settings.h"
#include "utils/Utils.h"

class SongList : public sf::Drawable {
    mutable std::vector<Tile> tiles;
    mutable sf::Time t;
    mutable int shift = 0;
    mutable int vel = 0;
    mutable int cur = -1;
    const int accel = 1500;
    const int FAST = 2000;
    const int TILE_GAP = 2;
    const int MAX_VEL = 3000;
    const int MAX_VEL_WITH_SHIFT = 30000;
    mutable int last_y = 0;



    mutable Player* player;
    PicLoader& picLoader;

    int get_low_tile() const;

    int get_up_tile(int winh) const;

    const int CUR_SHIFT = 50;

public:
    mutable bool holding = false;
    mutable bool shift_pressed = false;

    mutable sf::Vector2<unsigned int> winsz;

    explicit SongList(PicLoader&);

    void init(Player* p, const std::wstring& filter = L"", const std::wstring& album = L"");

    void norm_shift() const;

    void norm_shift_up() const;

    void norm_shift_down() const;

    void scroll(int delta) const;

    void update_shift() const;

    void update_vel(sf::Time t) const;

    std::pair<int, int> get_click_id(int x, int y) const;

    void grab(int y);

    void release(int y, sf::Time time);

    void set_position(int y);

    void norm_shift_tile() const;

    void pageup();

    void pagedown();

    size_t size();

    void play_next(bool ignore_loop = false);

    void play_prev(bool ignore_loop = false);

    void find_cur();

protected:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

#endif  // XYLON_SONGLIST_H
