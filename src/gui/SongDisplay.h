#ifndef XYLON_SONGDISPLAY_H
#define XYLON_SONGDISPLAY_H

#include <SFML/Graphics/Text.hpp>

#include "../Player.h"
#include "../Tile.h"
#include "ProgressBarView.h"

class SongDisplay : public sf::Drawable {
    mutable sf::Text title_text;
    mutable sf::Text artist_text;
    mutable uint64_t previousId = -1;
    void update(Song& song) const;

public:
    static const int PIC = 300;
    Player* player;
    mutable ProgressBarView bar;

    mutable int margin_x, margin_y;

    explicit SongDisplay(Player* _p);

    ~SongDisplay() override;

protected:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

#endif  // XYLON_SONGDISPLAY_H
