#ifndef SONGVIEW
#define SONGVIEW

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "Tile.cpp"
#include "Player.cpp"

class SongView {
    std::vector<Tile *> tiles;
    int shift = 0;
    sf::Vector2<unsigned int> winsz;
    Player *pl;

    int get_low_tile() {
        return std::max(0, (-shift) / Tile::W);
    }

    int get_up_tile(int winh) {
        return std::min(int(tiles.size()), (-shift + winh) / Tile::H + 1);
    }

public:
    void render(sf::RenderWindow &window, sf::Font &font) {
        sf::RectangleShape sh;
        winsz = window.getSize();
        sh.setSize(sf::Vector2f(Tile::W, Tile::H));
        int low = get_low_tile();
        int up = get_up_tile(winsz.y);
        int cur = pl->current_index();
        sh.setOutlineColor(sf::Color(sf::Color::Green));
        for (int i = low; i < up; ++i) {
            if (i == cur) {
                continue;
            }
            tiles[i]->position = sf::Vector2i(300, shift + i * Tile::H);
            tiles[i]->render(window, font, sh);
        }
        if (low <= cur && cur < up) {
            sh.setOutlineColor(sf::Color(sf::Color::Blue));
            tiles[cur]->position = sf::Vector2i(250, shift + cur * Tile::H);
            tiles[cur]->render(window, font, sh);
        }
    }

    void init(Player *p) {
        pl = p;
        for (auto &it: p->songs) {
            tiles.push_back(new Tile(it));
        }
        std::cerr << "SIZE: " << tiles.size() << '\n';
    }

    void scroll(int delta) {
        shift += delta * 30;
        shift = std::min(shift, 30);
        shift = std::max(shift, -int(tiles.size() * Tile::H) - 30 + int(winsz.y));
    }

    int get_click_index(int x, int y) {
        for (int i = get_low_tile(); i < get_up_tile(winsz.y); ++i) {
            if (tiles[i]->position.x <= x && tiles[i]->position.x + Tile::W >= x) {
                if (tiles[i]->position.y <= y && tiles[i]->position.y + Tile::H >= y) {
                    return i;
                }
            }
        }
        return -1;
    }
};

#endif