#ifndef XYLON_MOUSETRACE_H
#define XYLON_MOUSETRACE_H

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Shape.hpp>
#include <queue>

class MouseTrace : public sf::Drawable {
    mutable std::deque<sf::Vector2i> positions;
    mutable sf::VertexArray triangleStrip;
    mutable int size;

    void constructPolygon() const;

public:
    float radius = 6;
    const int maxLength = 16;
    bool clicked = false;

    void push(sf::Vector2<int> position);

    void setClicked(bool b);

    MouseTrace();
    ~MouseTrace() override;

protected:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

#endif  // XYLON_MOUSETRACE_H
