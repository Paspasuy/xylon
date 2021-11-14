#ifndef SONGVIEW
#define SONGVIEW

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "Tile.cpp"
#include "Player.cpp"
#include "ProgressBarView.cpp" //TODO: .h file

class SongView {
    std::vector<Tile *> tiles;
    int shift = 0;
    Player *pl;

    int get_low_tile() {
        return std::max(0, (-shift) / Tile::W);
    }

    int get_up_tile(int winh) {
        return std::min(int(tiles.size()), (-shift + winh) / Tile::H + 1);
    }

    const int CUR_SHIFT = 50;
    ProgressBarView bar;
public:
    sf::Vector2<unsigned int> winsz;

    void render(sf::RenderWindow &window, sf::Font &font) {
        sf::RectangleShape sh;
        winsz = window.getSize();
        sh.setSize(sf::Vector2f(Tile::W, Tile::H - 2));
        int low = get_low_tile();
        int up = get_up_tile(winsz.y);
        int cur = pl->current_index();
        sh.setOutlineColor(sf::Color(255, 127, 0));
        for (int i = low; i < up; ++i) {
            if (i == cur) {
                continue;
            }
            tiles[i]->position = sf::Vector2i(winsz.x - Tile::W, shift + i * Tile::H);
            tiles[i]->render(window, font, sh, 0);
        }
        sh.setOutlineColor(sf::Color(sf::Color::Blue));
        tiles[cur]->position = sf::Vector2i(winsz.x - Tile::W - CUR_SHIFT, shift + cur * Tile::H);
        tiles[cur]->render(window, font, sh, 1);
        int margin = tiles[cur]->margin;
        bar.update(pl);
        bar.render(window, font, margin, Tile::PIC + 57 + margin / 2);
    }

    void init(Player *p) {
        pl = p;
        tiles.clear();
        for (auto &it: p->songs) {
            tiles.push_back(new Tile(it));
        }
        shift = (-pl->ptr + 1) * Tile::H;
        norm_shift();
        std::cerr << "SHIFT: " << shift << std::endl;
        std::cout << "SIZE: " << tiles.size() << '\n';
    }

    void norm_shift() {
        shift = std::max(shift, -int(tiles.size() * Tile::H) - 30 + int(winsz.y));
        shift = std::min(shift, 30);
    }

    void norm_shift_up() {
        int up_bound = -(pl->current_index() - 1) * Tile::H;
        shift = std::max(shift, up_bound);
    }

    void norm_shift_down() {
        int down_bound = -(pl->current_index() + 2) * Tile::H + winsz.y;
        shift = std::min(shift, down_bound);
    }

    void scroll(int delta) {
        shift += delta * 30;
        norm_shift();
    }

    int get_click_id(int x, int y) {
        for (int i = get_low_tile(); i < get_up_tile(winsz.y); ++i) {
            if (tiles[i]->position.x <= x && tiles[i]->position.x + Tile::W >= x) {
                if (tiles[i]->position.y <= y && tiles[i]->position.y + Tile::H >= y) {
                    return tiles[i]->s->id;
                }
            }
        }
        return -1;
    }
};

#endif