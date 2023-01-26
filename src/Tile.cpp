#include "Tile.h"

#include <SFML/Graphics/Text.hpp>

#include "utils/Utils.h"

Tile::Tile(Song* s)
    : s(s),
      title_text(s->title, BOLD_FONT, 16),
      artist_text(s->artist, FONT, 14),
      time_text(secondsToTimeString(s->getDuration().asSeconds()), FONT, 14),
      album_text(s->album, FONT, 14) {
    //        TagLib::MP4::Tag* tag = ff.tag();
    //        auto itemsListMap = tag->itemListMap();
    //        TagLib::MP4::Item coverItem = itemsListMap["covr"];
    //        TagLib::MP4::CoverArtList coverArtList = coverItem.toCoverArtList();
}

void Tile::render(sf::RenderTarget& target, PicLoader& pl, sf::RectangleShape& sh, bool too_fast,
                  bool is_cur) {
    if (pl.should_load && !too_fast) {
        if (!s->pic_loaded && !s->pic_loading) {
            pl.load(s);
        }
    }
    sh.setPosition(position.x, position.y);
    sh.setOutlineThickness(1.f);
    title_text.setPosition(sh.getPosition().x + 2 + 85, sh.getPosition().y + 2);
    artist_text.setPosition(sh.getPosition().x + 2 + 85, sh.getPosition().y + 20);
    time_text.setPosition(sh.getPosition().x + 2 + 85, sh.getPosition().y + Tile::H - 20);
    album_text.setPosition(
        sh.getPosition().x +
            std::max(-4 - (int)s->album.size() * 8 + (int)sh.getSize().x, 90 + 8 * 6),
        sh.getPosition().y + Tile::H - 20);
    int alpha = is_cur ? 255 : 180;
    title_text.setFillColor(sf::Color(255, 255, 255, alpha));
    artist_text.setFillColor(sf::Color(255, 255, 255, alpha));
    album_text.setFillColor(sf::Color(255, 255, 255, alpha));
    if (s->pic_loaded && s->small_sprite != nullptr) {
        s->small_sprite->setPosition(position.x, position.y);
        float transition = std::min(
            1.f, static_cast<float>(clk.getElapsedTime().asMilliseconds() - s->time_loaded) /
                     FADE_TIME);
        if (!is_cur && !pic_transparent) {
            s->small_sprite->setColor(
                sf::Color(255, 255, 255, static_cast<int>(IMG_ALPHA * transition)));
            if (transition == 1.f) pic_transparent = true;
        }
        if (is_cur && pic_transparent) {
            s->small_sprite->setColor(sf::Color(255, 255, 255, static_cast<int>(255 * transition)));
            if (transition == 1.f) pic_transparent = false;
        }
    }
    target.draw(sh);
    if (s->small_sprite) {
        target.draw(*s->small_sprite);
    }
    target.draw(title_text);
    target.draw(artist_text);
    target.draw(album_text);
    target.draw(time_text);
}

int Tile::W;
int Tile::MAX_W = 450;
