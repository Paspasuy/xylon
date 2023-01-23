#ifndef XYLON_SONGDISPLAY_H
#define XYLON_SONGDISPLAY_H

#include <SFML/Graphics/Text.hpp>

#include "Player.h"
#include "ProgressBarView.h"
#include "Tile.h"

class SongDisplay {
public:
    static const int PIC = 300;
    Player* p;
    ProgressBarView bar;
    int margin_x, margin_y;

    SongDisplay(Player* _p);

    void render(sf::RenderWindow& window);
};

#endif  // XYLON_SONGDISPLAY_H
