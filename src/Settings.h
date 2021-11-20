//
// Created by pavel on 11/19/21.
//

#ifndef XYLON_SETTINGS_H
#define XYLON_SETTINGS_H


#include <fstream>
#include <SFML/Graphics/Color.hpp>

class Settings {
public:
    int vis_type = 0;
    sf::Color c1, c2, c3, c4, c5, c6;
    void init_col(sf::Color *c, const int *colors);
    void init();
};


#endif //XYLON_SETTINGS_H
