#ifndef XYLON_VOLUMECIRCLESLIDER_H
#define XYLON_VOLUMECIRCLESLIDER_H

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include "../Player.h"
#include "VolumeShape.h"

class VolumeCircleSlider : public sf::Drawable {
public:
    Player* p;
    sf::Time t;
    mutable VolumeShape shape;

    ~VolumeCircleSlider() override;

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

public:
    explicit VolumeCircleSlider(Player* _p);

    void touch(sf::Time _t);
};

#endif  // XYLON_VOLUMECIRCLESLIDER_H
