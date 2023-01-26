#include "SongDisplay.h"

SongDisplay::SongDisplay(Player* _p) { player = _p; }

SongDisplay::~SongDisplay() {}

void SongDisplay::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    Song& song = player->getSong();
    update(song);
    margin_x = std::max(0, int(target.getSize().x - Tile::W - PIC) / 2);
    margin_y = std::max(0, int(target.getSize().y / 2 - PIC) / 2);

    if (!song.pic_loaded) {
        song.load_pic();
    }
    if (song.sprite != nullptr) {
        song.sprite->setPosition(margin_x, margin_y);
        target.draw(*song.sprite);
    }

    int w = 10;
    int sw = margin_x + (PIC - w * song.title.size()) / 2;
    title_text.setPosition(sw, PIC + margin_y + 12);
    target.draw(title_text);

    w = 8;
    sw = margin_x + (PIC - w * song.artist.size()) / 2;
    artist_text.setPosition(sw, PIC + margin_y + 20 + 12);
    target.draw(artist_text);

    bar.update(player);
    bar.setPosition(margin_x, PIC + 57 + margin_y);
    target.draw(bar);
}

void SongDisplay::update(Song& song) const {
    if (song.id != previousId) {
        previousId = song.id;
        title_text = sf::Text(song.title, BOLD_FONT, 16);
        artist_text = sf::Text (song.artist, FONT, 14);
    }
}
