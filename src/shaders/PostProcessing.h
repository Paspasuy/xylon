#ifndef XYLON_POSTPROCESSING_H
#define XYLON_POSTPROCESSING_H

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Shader.hpp>
#include <SFML/Graphics/Drawable.hpp>

class PostProcessing : public sf::Drawable {
    mutable sf::RenderStates shader_states;
    mutable sf::Shader blur_shader;
    mutable sf::Shader assemble_shader;

    mutable sf::RenderTexture scene_render;
    mutable sf::RenderTexture blur_render;
    mutable sf::ContextSettings ctx;
public:
    PostProcessing(const sf::Vector2f& size, sf::ContextSettings ctx);
    void add(sf::Drawable& drawable);
    void clear();

    ~PostProcessing() override;
    void create(const sf::Vector2f& size);

protected:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

#endif  // XYLON_POSTPROCESSING_H
