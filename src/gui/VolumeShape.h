//
// Created by pavel on 11/16/21.
//

#ifndef XYLON_VOLUMESHAPE_H
#define XYLON_VOLUMESHAPE_H

#include <SFML/Graphics.hpp>
#include <valarray>

class VolumeShape : public sf::Shape {
public:
    int prog = -1;

    explicit VolumeShape(float radius);

    virtual std::size_t getPointCount() const;

    virtual sf::Vector2f getPoint(std::size_t index) const;

    void touch(int val);

private:
    float r;
};

#endif  // XYLON_VOLUMESHAPE_H
