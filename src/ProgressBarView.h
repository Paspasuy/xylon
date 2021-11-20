#ifndef XYLON_PROGRESSBARVIEW_H
#define XYLON_PROGRESSBARVIEW_H

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Font.hpp>
#include "Player.h"
#include "Settings.h"

class ProgressBarView {
public:
    double progress;
    const int height = 3;
    int width;
    bool holding = 0;
    int current, duration;
    sf::RectangleShape cont;
    Settings *s;
    ProgressBarView(Settings *_s);

void update(Player *pl);

    void render(sf::RenderWindow &window, sf::Font &font, int x, int y);

    bool in_bar(int x, int y);

    void set_position(Player *p, int x);
};

#endif //XYLON_PROGRESSBARVIEW_H
