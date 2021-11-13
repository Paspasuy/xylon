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
        for (int i = get_low_tile(); i < get_up_tile(winsz.y); ++i) {
            int x = 300;
            sh.setOutlineColor(sf::Color(sf::Color::Green));
            if (i == pl->current_index()) {
                x -= 50;
                sh.setOutlineColor(sf::Color(sf::Color::Blue));
            }
            tiles[i]->position = sf::Vector2i(x, shift + i * Tile::H);
            sh.setPosition(tiles[i]->position.x, tiles[i]->position.y);
            sh.setOutlineThickness(2.f);
            sh.setFillColor(sf::Color(sf::Color::Black));
            sf::Text title_text(tiles[i]->title, font, 16);
            sf::Text artist_text(tiles[i]->artist, font, 14);
            title_text.setPosition(sh.getPosition().x + 2, sh.getPosition().y + 2);
            artist_text.setPosition(sh.getPosition().x + 2, sh.getPosition().y + 20);
            window.draw(sh);
            window.draw(title_text);
            window.draw(artist_text);
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
        shift = std::min(shift, 30);
        shift = std::max(shift, -int(tiles.size() * Tile::H) - 30 + int(winsz.y));
        shift += delta * 30;
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