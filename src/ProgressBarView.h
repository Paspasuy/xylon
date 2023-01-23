#ifndef XYLON_PROGRESSBARVIEW_H
#define XYLON_PROGRESSBARVIEW_H

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include "Player.h"
#include "Utils.h"

class ProgressBarView {
public:
    double progress;
    bool holding = false;
    int current, duration;
    sf::RectangleShape cont;

    void update(Player* pl);

    void render(sf::RenderWindow& window, int x, int y);

    bool in_bar(int x, int y);

    void set_position(Player* p, int x);
};

#endif  // XYLON_PROGRESSBARVIEW_H
