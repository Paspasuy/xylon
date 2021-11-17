#include "SongDisplay.h"

SongDisplay::SongDisplay(Player *_p) {
    p = _p;
}

void SongDisplay::render(sf::RenderWindow &window, sf::Font &font, sf::Font &bold_font) {
    Song *s = p->getSong();
    margin = std::min(std::max(0, int(window.getSize().x - Tile::W - PIC)) / 2,
                      std::max(0, int(window.getSize().y - PIC)));

    if (!s->pic_loaded) {
        s->load_pic();
    }

    s->sprite.setPosition(margin, margin / 2);
    window.draw(s->sprite);

    sf::Text title_text(s->title, bold_font, 16);
    sf::Text artist_text(s->artist, font, 14);
//    sf::Text album_text(s->album, font, 14);
    int w = 10;
    int sw = margin + (PIC - w * s->title.getSize()) / 2;
    title_text.setPosition(sw, PIC + margin * 0.5 + 12);
    window.draw(title_text);
    w = 8;
    sw = margin + (PIC - w * s->artist.getSize()) / 2;
    artist_text.setPosition(sw, PIC + margin * 0.5 + 20 + 12);
    window.draw(artist_text);

    bar.update(p);
    bar.render(window, font, margin, PIC + 57 + margin / 2);
}
