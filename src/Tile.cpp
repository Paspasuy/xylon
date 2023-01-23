#include "Tile.h"

#include <SFML/Graphics/Text.hpp>

#include "utils/Utils.h"

Tile::Tile(Song* s) : s(s) {
    //        TagLib::MP4::Tag* tag = ff.tag();
    //        auto itemsListMap = tag->itemListMap();
    //        TagLib::MP4::Item coverItem = itemsListMap["covr"];
    //        TagLib::MP4::CoverArtList coverArtList = coverItem.toCoverArtList();
}

void Tile::render(sf::RenderWindow& window, PicLoader& pl, sf::RectangleShape& sh, bool too_fast,
                  bool is_cur) {
    if (pl.should_load && !too_fast) {
        if (!s->pic_loaded && !s->pic_loading) {
            pl.load(s);
        }
    }
    sh.setPosition(position.x, position.y);
    sh.setOutlineThickness(1.f);
    sf::Text title_text(s->title, BOLD_FONT, 16);
    sf::Text artist_text(s->artist, FONT, 14);
    sf::Text time_text(secondsToTimeString(s->getDuration().asSeconds()), FONT, 14);
    sf::Text album_text(s->album, FONT, 14);
    title_text.setPosition(sh.getPosition().x + 2 + 85, sh.getPosition().y + 2);
    artist_text.setPosition(sh.getPosition().x + 2 + 85, sh.getPosition().y + 20);
    time_text.setPosition(sh.getPosition().x + 2 + 85, sh.getPosition().y + Tile::H - 20);
    album_text.setPosition(
        sh.getPosition().x +
            std::max(-4 - (int)s->album.getSize() * 8 + (int)sh.getSize().x, 90 + 8 * 6),
        sh.getPosition().y + Tile::H - 20);
    if (!is_cur) {
        title_text.setFillColor(sf::Color(255, 255, 255, 180));
        artist_text.setFillColor(sf::Color(255, 255, 255, 180));
        album_text.setFillColor(sf::Color(255, 255, 255, 180));
    }
    if (s->pic_loaded) {
        s->small_sprite.setPosition(position.x, position.y);
        float transition = std::min(
            1.f, static_cast<float>((clk.getElapsedTime() - s->time_loaded).asMilliseconds()) /
                     FADE_TIME);
        if (!is_cur && !pic_transparent) {
            s->small_sprite.setColor(
                sf::Color(255, 255, 255, static_cast<int>(IMG_ALPHA * transition)));
            if (transition == 1.f) pic_transparent = true;
        }
        if (is_cur && pic_transparent) {
            s->small_sprite.setColor(sf::Color(255, 255, 255, static_cast<int>(255 * transition)));
            if (transition == 1.f) pic_transparent = false;
        }
    }
    window.draw(sh);
    window.draw(s->small_sprite);
    window.draw(title_text);
    window.draw(artist_text);
    window.draw(album_text);
    window.draw(time_text);
}

int Tile::W;
int Tile::MAX_W = 450;
