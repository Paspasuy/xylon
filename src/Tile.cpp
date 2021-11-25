#include <SFML/Graphics/Text.hpp>
#include <thread>
#include "Tile.h"


Tile::Tile(Song *_s) {
        s = _s;
//        TagLib::MP4::Tag* tag = ff.tag();
//        auto itemsListMap = tag->itemListMap();
//        TagLib::MP4::Item coverItem = itemsListMap["covr"];
//        TagLib::MP4::CoverArtList coverArtList = coverItem.toCoverArtList();
}


void Tile::render(sf::RenderWindow &window, sf::Font &font, sf::Font &bold_font, sf::RectangleShape &sh, bool is_cur) {
    if (!s->pic_loaded) {
        if (!pics_locked) {
            pics_locked = true;
            to_load.push_back(s);
            std::thread thr(load_pics);
            thr.detach();
        }
    }
    sh.setPosition(position.x, position.y);
    sh.setOutlineThickness(1.f);
    sf::Text title_text(s->title, bold_font, 16);
    sf::Text artist_text(s->artist, font, 14);
    sf::Text album_text(s->album, font, 14);
    title_text.setPosition(sh.getPosition().x + 2 + 85, sh.getPosition().y + 2);
    artist_text.setPosition(sh.getPosition().x + 2 + 85, sh.getPosition().y + 20);
    album_text.setPosition(sh.getPosition().x - 4 - s->album.getSize() * 8 + sh.getSize().x, sh.getPosition().y + Tile::H - 20);
    if (!is_cur) {
        title_text.setFillColor(sf::Color(255, 255, 255, 180));
        artist_text.setFillColor(sf::Color(255, 255, 255, 180));
        album_text.setFillColor(sf::Color(255, 255, 255, 180));
    }
    if (s->pic_loaded) {
        s->small_sprite.setPosition(position.x, position.y);
        if (!is_cur && !pic_transparent) {
            s->small_sprite.setColor(sf::Color(255, 255, 255, 100));
            pic_transparent = true;
        }
        if (is_cur && pic_transparent) {
            s->small_sprite.setColor(sf::Color(255, 255, 255, 255));
            pic_transparent = false;
        }
    }
    window.draw(sh);
    window.draw(s->small_sprite);
    window.draw(title_text);
    window.draw(artist_text);
    window.draw(album_text);

}

int Tile::W;
std::vector<Song*> Tile::to_load;
bool Tile::pics_locked = false;
int Tile::MAX_W = 450;

void Tile::load_pics() {
    for (Song* ptr: to_load) {
        ptr->load_pic();
    }
    to_load.clear();
    pics_locked = false;
}
