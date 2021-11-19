#ifndef XYLON_SONGDISPLAY_H
#define XYLON_SONGDISPLAY_H

#include "Tile.h"
#include "ProgressBarView.h"
#include "Player.h"
#include <SFML/Graphics/Text.hpp> // у меня рекуррентный include


class SongDisplay {
public:
    static const int PIC = 300;
    Player *p;
    ProgressBarView bar;
    int margin_x, margin_y;

    SongDisplay(Player *_p);

    void render(sf::RenderWindow &window, sf::Font &font, sf::Font &bold_font);
};

#endif //XYLON_SONGDISPLAY_H
