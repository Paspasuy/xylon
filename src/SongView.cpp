#include "SongView.h"

int SongView::get_low_tile() {
    return std::max(0, (-shift) / Tile::W);
}

int SongView::get_up_tile(int winh) {
    return std::min(int(tiles.size()), (-shift + winh) / Tile::H + 1);
}

void SongView::render(sf::RenderWindow &window, sf::Font &font, sf::Font &bold_font, sf::Time time) {
    update_vel(time - t);
    update_shift(time - t);
    t = time;
    sf::RectangleShape sh;
    winsz = window.getSize();
    sh.setSize(sf::Vector2f(Tile::W, Tile::H - 2));
    int low = get_low_tile();
    int up = get_up_tile(winsz.y);
    int cur = pl->current_index();
    sh.setOutlineColor(sf::Color(255, 127, 0, 180));
    sh.setFillColor(sf::Color(255, 127, 0, 20));
    for (int i = low; i < up; ++i) {
        if (i == cur) {
            continue;
        }
        tiles[i]->position = sf::Vector2i(winsz.x - Tile::W, shift + i * Tile::H);
        tiles[i]->render(window, font, bold_font, sh, false);
    }
    if (cur != -1) {
        sh.setOutlineColor(sf::Color(sf::Color::Blue));
        sh.setFillColor(sf::Color(0, 0, 255, 20));
        tiles[cur]->position = sf::Vector2i(winsz.x - Tile::W - CUR_SHIFT, shift + cur * Tile::H);
        tiles[cur]->render(window, font, bold_font, sh, true);
    }
}

void SongView::init(Player *p) {
    pl = p;
    for (auto iter: tiles) {
        delete iter;
    }
    tiles.clear();
    for (auto &it: p->songs) {
        tiles.push_back(new Tile(it));
    }
    shift = (-pl->ptr + 1) * Tile::H;
    norm_shift();
}

void SongView::norm_shift() {
    shift = std::max(shift, -int(tiles.size() * Tile::H) - 30 + int(winsz.y));
    shift = std::min(shift, 30);
}

void SongView::norm_shift_up() {
    int up_bound = -(pl->current_index() - 1) * Tile::H;
    shift = std::max(shift, up_bound);
}

void SongView::norm_shift_down() {
    int down_bound = -(pl->current_index() + 2) * Tile::H + winsz.y;
    shift = std::min(shift, down_bound);
}

void SongView::scroll(int delta) {
    vel += delta * 250;
    norm_shift();
}

std::pair<int, int> SongView::get_click_id(int x, int y) {
    for (int i = get_low_tile(); i < get_up_tile(winsz.y); ++i) {
        if (tiles[i]->position.x <= x && tiles[i]->position.x + Tile::W >= x) {
            if (tiles[i]->position.y <= y && tiles[i]->position.y + Tile::H >= y) {
                return {tiles[i]->s->id, i};
            }
        }
    }
    return {-1, -1};
}

void SongView::update_vel(sf::Time t) {
    if (vel > 0) {
        vel -= t.asSeconds() * accel;
        vel = std::max(0, vel);
        vel = std::min(MAX_VEL, vel);
    } else if (vel < 0) {
        vel += t.asSeconds() * accel;
        vel = std::min(0, vel);
        vel = std::max(-MAX_VEL, vel);
    }
}

void SongView::update_shift(sf::Time t) {
    if (!holding) {
        shift += vel * t.asSeconds();
        norm_shift();
    } else {

    }
}

void SongView::grab(int y) {
    holding = 1;
    last_y = y;
    vel = 0;
}

void SongView::release(int y, sf::Time time) {
    vel = (y - last_y) / (time - t).asSeconds();
    holding = 0;
}

void SongView::set_position(int y) {
    shift += y - last_y;
    last_y = y;
}

