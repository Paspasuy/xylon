#ifndef XYLON_VISUALISER_H
#define XYLON_VISUALISER_H

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include "../utils/Settings.h"

class Visualiser : public sf::Drawable {
protected:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

public:
    ~Visualiser() override;

    bool display = true;
    mutable float fft[2048];
};

#endif  // XYLON_VISUALISER_H
