//
// Created by pavel on 11/19/21.
//

#ifndef XYLON_VISUALISER_H
#define XYLON_VISUALISER_H

#include <SFML/Graphics/RenderWindow.hpp>

class Visualiser {
public:
    bool display = 1;
    Visualiser();

    void render(sf::RenderWindow &window, float *fft);
};


#endif //XYLON_VISUALISER_H
