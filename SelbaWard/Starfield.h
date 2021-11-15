// Selba Ward (https://github.com/Hapaxia/SelbaWard)

#ifndef XYLON_STARFIELD_H
#define XYLON_STARFIELD_H

#include "Common.h"

namespace selbaward
{

// SW Starfield v1.1.1
    class Starfield : public sf::Drawable, public sf::Transformable
    {
    public:
        Starfield(sf::Vector2f size = { 0.f, 0.f }, unsigned int numberOfStars = 100u, sf::Color color = sf::Color(160, 160, 160));
        void regenerate();
        void regenerate(sf::Vector2f size);
        void regenerate(sf::Vector2f size, unsigned int numberOfStars);
        void regenerate(unsigned int numberOfStars);

        void setColor(sf::Color color);

        void move(sf::Vector2f movement);

    private:
        sf::PrimitiveType m_primitiveType;
        std::vector<sf::Vertex> m_vertices;
        sf::Vector2f m_size;
        sf::Color m_color;

        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    };

} // namespace selbaward

#endif //XYLON_STARFIELD_H
