#include "SongDisplay.h"

SongDisplay::SongDisplay(Player* _p, Settings* _s) : bar(ProgressBarView(_s)) { p = _p; }

void SongDisplay::render(sf::RenderWindow& window, sf::Clock& cl, sf::Font& font, sf::Font& bold_font) {
    Song& s = p->getSong();
    margin_x = std::max(0, int(window.getSize().x - Tile::W - PIC) / 2);
    margin_y = std::max(0, int(window.getSize().y / 2 - PIC) / 2);

    if (!s.pic_loaded) {
        s.load_pic(&cl);
    }

    s.sprite.setPosition(margin_x, margin_y);
    window.draw(s.sprite);

    sf::Text title_text(s.title, bold_font, 16);
    sf::Text artist_text(s.artist, font, 14);
    //    sf::Text album_text(s.album, font, 14);
    int w = 10;
    int sw = margin_x + (PIC - w * s.title.getSize()) / 2;
    title_text.setPosition(sw, PIC + margin_y + 12);
    window.draw(title_text);
    w = 8;
    sw = margin_x + (PIC - w * s.artist.getSize()) / 2;
    artist_text.setPosition(sw, PIC + margin_y + 20 + 12);
    window.draw(artist_text);

    bar.update(p);
    bar.render(window, font, margin_x, PIC + 57 + margin_y);
}
