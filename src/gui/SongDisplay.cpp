#include "SongDisplay.h"

SongDisplay::SongDisplay(Player* _p) { p = _p; }

SongDisplay::~SongDisplay() {}

void SongDisplay::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    Song& s = p->getSong();
    margin_x = std::max(0, int(target.getSize().x - Tile::W - PIC) / 2);
    margin_y = std::max(0, int(target.getSize().y / 2 - PIC) / 2);

    if (!s.pic_loaded) {
        s.load_pic();
    }

    s.sprite.setPosition(margin_x, margin_y);
    target.draw(s.sprite);

    sf::Text title_text(s.title, BOLD_FONT, 16);
    sf::Text artist_text(s.artist, FONT, 14);
    //    sf::Text album_text(s.album, FONT, 14);
    int w = 10;
    int sw = margin_x + (PIC - w * s.title.getSize()) / 2;
    title_text.setPosition(sw, PIC + margin_y + 12);
    target.draw(title_text);

    w = 8;
    sw = margin_x + (PIC - w * s.artist.getSize()) / 2;
    artist_text.setPosition(sw, PIC + margin_y + 20 + 12);
    target.draw(artist_text);

    bar.update(p);
    bar.setPosition(margin_x, PIC + 57 + margin_y);
    target.draw(bar);
}
