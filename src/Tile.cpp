#include <SFML/Graphics/Text.hpp>
#include "Tile.h"


Tile::Tile(Song *_s) {
        s = _s;
//        TagLib::MP4::Tag* tag = ff.tag();
//        auto itemsListMap = tag->itemListMap();
//        TagLib::MP4::Item coverItem = itemsListMap["covr"];
//        TagLib::MP4::CoverArtList coverArtList = coverItem.toCoverArtList();
}

void Tile::render(sf::RenderWindow &window, sf::Font &font, sf::Font &bold_font, sf::RectangleShape &sh) {
    sh.setPosition(position.x, position.y);
    sh.setOutlineThickness(2.f);
//    sh.setFillColor(sf::Color(sf::Color::Transparent));
    sf::Text title_text(s->title, bold_font, 16);
    sf::Text artist_text(s->artist, font, 14);
    sf::Text album_text(s->album, font, 14);
    title_text.setPosition(sh.getPosition().x + 2, sh.getPosition().y + 2);
    artist_text.setPosition(sh.getPosition().x + 2, sh.getPosition().y + 20);
    album_text.setPosition(sh.getPosition().x - 4 - s->album.getSize() * 8 + Tile::W, sh.getPosition().y + Tile::H - 20);
    window.draw(sh);
    window.draw(title_text);
    window.draw(artist_text);
    window.draw(album_text);

}

int Tile::W;
int Tile::MAX_W = 400;