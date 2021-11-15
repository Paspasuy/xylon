#ifndef XYLON_VOLUMECIRCLESLIDER_H
#define XYLON_VOLUMECIRCLESLIDER_H

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Font.hpp>
#include "Player.h"

class VolumeCircleSlider {
public:
    Player *p;
    sf::Time t;
    int val = 100;

    VolumeCircleSlider(Player *_p, sf::Time _t);

    void render(sf::RenderWindow &window, sf::Font &bold_font, sf::Time t);

    void touch(sf::Time _t);

};


#endif //XYLON_VOLUMECIRCLESLIDER_H
