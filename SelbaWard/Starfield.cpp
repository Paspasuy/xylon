// Selba Ward (https://github.com/Hapaxia/SelbaWard)

#include "Starfield.h"

#include <random>
#include <functional>

namespace
{

    std::mt19937 randomGenerator;
    std::uniform_int_distribution<unsigned short int> randomDistributionAlpha(1u, 255u);
    std::function <unsigned short int()> randomAlpha;

    inline void randomSeed()
    {
        std::random_device rd;
        randomGenerator.seed(rd());
        randomAlpha = std::bind(randomDistributionAlpha, randomGenerator);
    }

    inline float randomValue(const float low, const float high)
    {
        return std::uniform_real_distribution<float>{low, high}(randomGenerator);
    }

} // namespace

namespace selbaward
{

    Starfield::Starfield(const sf::Vector2f size, const unsigned int numberOfStars, const sf::Color color)
            : m_primitiveType{ sf::PrimitiveType::Points }
            , m_vertices(numberOfStars)
            , m_size(size)
            , m_color(color)
    {
        randomSeed();
        regenerate();
    }

    void Starfield::move(sf::Vector2f movement)
    {
        for (auto& vertex : m_vertices)
        {
            // move
            vertex.position += movement * (static_cast<float>(vertex.color.a) / 255.f);

            // wrap
            if (vertex.position.x < 0)
                vertex.position = { m_size.x, randomValue(0.f, m_size.y), };
            else if (vertex.position.x > m_size.x)
                vertex.position = { 0.f, randomValue(0.f, m_size.y), };
            if (vertex.position.y < 0)
                vertex.position = { randomValue(0.f, m_size.x), m_size.y, };
            else if (vertex.position.y > m_size.y)
                vertex.position = { randomValue(0.f, m_size.x) , 0.f };
        }
    }

    void Starfield::regenerate()
    {
        for (auto& vertex : m_vertices)
        {
            vertex.position = { randomValue(0.f, m_size.x), randomValue(0.f, m_size.y) };
            vertex.color = m_color;
            vertex.color.a = static_cast<sf::Uint8>(randomAlpha());
        }
    }

    void Starfield::regenerate(const sf::Vector2f size)
    {
        m_size = size;
        regenerate();
    }

    void Starfield::regenerate(const sf::Vector2f size, const unsigned int numberOfStars)
    {
        m_vertices.resize(numberOfStars);
        regenerate(size);
    }

    void Starfield::regenerate(const unsigned int numberOfStars)
    {
        regenerate(m_size, numberOfStars);
    }

    void Starfield::setColor(const sf::Color color)
    {
        m_color = color;
        for (auto& vertex : m_vertices)
        {
            const sf::Uint8 alphaDepth{ vertex.color.a };
            vertex.color = m_color;
            vertex.color.a = alphaDepth;
        }
    }



// PRIVATE

    void Starfield::draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        states.transform *= getTransform();
        const unsigned int size{ static_cast<unsigned int>(m_vertices.size()) };
        if (size > 0)
            target.draw(&m_vertices.front(), size, m_primitiveType, states);
    }

} // namespace selbaward