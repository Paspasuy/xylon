#include "SongList.h"

#include <cmath>

int SongList::get_low_tile() const { return std::max(0, (-shift) / (Tile::H + TILE_GAP)); }

int SongList::get_up_tile(int winh) const {
    return std::min(int(tiles.size()), (-shift + winh) / (Tile::H + TILE_GAP) + 1);
}

void SongList::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    sf::Time time = clk.getElapsedTime();
    update_vel(sf::seconds(1.f/settings.framerateLimit));
    update_shift();
    t = time;
    sf::RectangleShape sh;
    winsz = target.getSize();
    sh.setSize(sf::Vector2f(Tile::W, Tile::H - 2));
    int low = get_low_tile();
    int up = get_up_tile(winsz.y);
    uint64_t cur_id = player->current_id();
    bool touch = false;
    sh.setOutlineColor(settings.inactiveTileOutline);
    sh.setFillColor(settings.inactiveTileBackground);
    int elapsed = clk.getElapsedTime().asMilliseconds();
    for (int i = low; i < up; ++i) {
        if (tiles[i].s->id == cur_id) {
            cur = i;
            touch = true;
            continue;
        }
        int tile_h = shift + i * (Tile::H + TILE_GAP);
        //        float dist = abs((tile_h + Tile::H / 2 - 1) - window.getSize().y / 2) + Tile::H *
        //        2; dist /= Tile::H;
        int xs = 0;  // int(CUR_SHIFT / dist);
        tiles[i].position = sf::Vector2i(winsz.x - Tile::W - xs, tile_h);
        sh.setSize(sf::Vector2f(Tile::W + xs, Tile::H - 2));
        tiles[i].render(target, picLoader, sh, elapsed, abs(vel) >= FAST, false);
    }
    if (touch) {
        sh.setOutlineColor(settings.activeTileOutline);
        sh.setFillColor(settings.activeTileBackground);
        sh.setSize(sf::Vector2f(Tile::W + CUR_SHIFT, Tile::H - 2));
        tiles[cur].position =
            sf::Vector2i(winsz.x - Tile::W - CUR_SHIFT, shift + cur * (Tile::H + TILE_GAP));
        tiles[cur].render(target, picLoader, sh, elapsed, abs(vel) >= FAST, true);
        if (player->loop) {
            //            int r = 7;
            sf::RectangleShape rep(sf::Vector2f(3, Tile::H));
            //            sf::CircleShape rep(r);
            rep.setFillColor(settings.repeatColor);
            auto pos = tiles[cur].position;
            //            pos.y += Tile::H / 2 - r;
            pos.x += Tile::W + CUR_SHIFT - 3;

            rep.setPosition(sf::Vector2f(pos));
            target.draw(rep);
        }
    }
}

void SongList::init(Player* p, const std::wstring& filter) {
    player = p;
    tiles.clear();
    for (Song* it : p->get_songs(filter)) {
        tiles.emplace_back(Tile(it));
    }
    find_cur();
    shift = (-player->ptr + 1) * (Tile::H + TILE_GAP);
    norm_shift();
}

void SongList::norm_shift() const {
    shift = std::max(shift, -int(tiles.size() * (Tile::H + TILE_GAP)) - 30 + int(winsz.y));
    shift = std::min(shift, 30);
}

void SongList::norm_shift_tile() const {
    vel = 0;
    norm_shift_up();
    norm_shift_down();
    norm_shift();
}

void SongList::norm_shift_up() const {
    int up_bound = -(cur - 1) * (Tile::H + TILE_GAP);
    shift = std::max(shift, up_bound);
}

void SongList::norm_shift_down() const {
    int down_bound = -(cur + 2) * (Tile::H + TILE_GAP) + winsz.y;
    shift = std::min(shift, down_bound);
}

void SongList::scroll(int delta) const {
    vel += delta * 250;
    norm_shift();
}

std::pair<int, int> SongList::get_click_id(int x, int y) const {
    for (int i = get_low_tile(); i < get_up_tile(winsz.y); ++i) {
        if (tiles[i].position.x - TILE_GAP <= x) {
            if (tiles[i].position.y - TILE_GAP / 2 <= y &&
                tiles[i].position.y + Tile::H + TILE_GAP / 2 >= y) {
                return {tiles[i].s->id, i};
            }
        }
    }
    return {-1, -1};
}

void SongList::update_vel(sf::Time t) const {
    int mx = shift_pressed ? MAX_VEL_WITH_SHIFT : MAX_VEL;
    if (vel > 0) {
        vel -= t.asSeconds() * accel;
        vel = std::max(0, vel);
        vel = std::min(mx, vel);
    } else if (vel < 0) {
        vel += t.asSeconds() * accel;
        vel = std::min(0, vel);
        vel = std::max(-mx, vel);
    }
}

void SongList::update_shift() const {
    if (!holding) {
        shift += vel / settings.framerateLimit;;
        norm_shift();
    }
}

void SongList::grab(int y) {
    holding = 1;
    last_y = y;
    vel = 0;
}

void SongList::release(int y, sf::Time time) {
    vel = (y - last_y) / (time - t).asSeconds();
    holding = 0;
}

void SongList::set_position(int y) {
    shift += y - last_y;
    last_y = y;
}

void SongList::pageup() {
    shift += 500;
    norm_shift();
}

void SongList::pagedown() {
    shift -= 500;
    norm_shift();
}

size_t SongList::size() { return tiles.size(); }

void SongList::play_prev(bool ignore_loop) {
    if (player->loop && !ignore_loop) {
        player->play();
    } else {
        player->play_id(tiles[cur = (cur == -1 ? 0 : (cur + tiles.size() - 1) % tiles.size())].s->id);
        norm_shift_tile();
    }
}

void SongList::play_next(bool ignore_loop) {
    if (player->loop && !ignore_loop) {
        player->play();
    } else {
        player->play_id(tiles[cur = (cur == -1 ? 0 : (cur + 1) % tiles.size())].s->id);
        norm_shift_tile();
    }
}

void SongList::find_cur() {
    int current_id = player->current_id();
    int index = 0;
    for (Tile& tile : tiles) {
        if (tile.s->id == current_id) {
            cur = index;
            return;
        }
        ++index;
    }
}

SongList::SongList(PicLoader& picLoader) : picLoader(picLoader) {}
