//
// Created by pavel on 11/19/21.
//

#ifndef XYLON_VISUALISER_H
#define XYLON_VISUALISER_H

#include <SFML/Graphics/RenderWindow.hpp>

#include "Settings.h"

class Visualiser {
public:
    bool display = true;
    Settings* s;
    float fft[2048];

    Visualiser(Settings* _s);

    void render(sf::RenderWindow& window);
};

#endif  // XYLON_VISUALISER_H
