#include "PostProcessing.h"

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include "../utils/Utils.h"

const char* ASSEMBLE_FRAG = "uniform sampler2D texture;\n"
    "uniform sampler2D add_texture;\n"
    "uniform float sum;\n"
    "uniform float add_weight;\n"
    "void main(void){\n"
    "    vec4 tex_color = texture2D(texture, gl_TexCoord[0].xy) * (sum - add_weight);\n"
    "    vec4 add_color = texture2D(add_texture, gl_TexCoord[0].xy) * add_weight;\n"
    "    gl_FragColor = tex_color + add_color;\n"
    "}";

const char* BOXBLUR_FRAG = "uniform sampler2D texture;\n"
    "uniform float texture_inverse;\n"
    "uniform int blur_radius;\n"
    "uniform vec2 blur_direction;\n"
    "void main(void){\n"
    "    vec4 sum = texture2D(texture, gl_TexCoord[0].xy);\n"
    "    for (int i = 0; i < blur_radius; i += 2){\n"
    "        sum += texture2D(texture, gl_TexCoord[0].xy + (float(i) * texture_inverse) * blur_direction);\n"
    "        sum += texture2D(texture, gl_TexCoord[0].xy - (float(i) * texture_inverse) * blur_direction);\n"
    "    }\n"
    "    gl_FragColor = sum / float(blur_radius + 1);\n"
    "}";

const char* LUMINISCENCE_FRAG = "uniform sampler2D texture;\n"
    "uniform float threshold;\n"
    "void main(void){\n"
    "    vec3 current_color = texture2D(texture, gl_TexCoord[0].xy).rgb;\n"
    "    vec4 pixel =  vec4(1.0, 0.0, 1.0, 0.0);\n"
    "    float brightness = dot(current_color.rgb, vec3(1.0, 1.0, 1.0));\n"
    "    if (brightness >= threshold){\n"
    "        pixel = texture2D(texture, gl_TexCoord[0].xy);\n"
    "    }\n"
    "    gl_FragColor = pixel;\n"
    "}";

PostProcessing::PostProcessing(const sf::Vector2f& size, sf::ContextSettings ctx) : ctx(ctx) {

    luminescence_shader.loadFromMemory(LUMINISCENCE_FRAG, sf::Shader::Fragment);
    luminescence_shader.setUniform("texture", sf::Shader::CurrentTexture);
    luminescence_shader.setUniform("threshold", 0.25f);

    blur_shader.loadFromMemory(BOXBLUR_FRAG, sf::Shader::Fragment);
    blur_shader.setUniform("texture", sf::Shader::CurrentTexture);
    blur_shader.setUniform("texture_inverse", 1.0f / size.x);

    assemble_shader.loadFromMemory(ASSEMBLE_FRAG, sf::Shader::Fragment);
    assemble_shader.setUniform("texture", sf::Shader::CurrentTexture);
    create(size);
}

PostProcessing::~PostProcessing() = default;

void PostProcessing::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    scene_render.display();
    if (!settings.enableGlow) {
        target.draw(sf::Sprite(scene_render.getTexture()), sf::BlendAdd);
        return;
    }
    shader_states.shader = &luminescence_shader;
    luminescence_render.clear(sf::Color(255, 255, 255, 0));
    luminescence_render.draw(sf::Sprite(scene_render.getTexture()), shader_states);
    luminescence_render.display();

    shader_states.shader = &blur_shader;
    blur_shader.setUniform("blur_radius", 16);

    blur_render.clear(sf::Color(0, 0, 0, 0));
    blur_render.draw(sf::Sprite(luminescence_render.getTexture()));
    blur_render.display();
    for (size_t it = 0; it < 3; ++it) {
        blur_shader.setUniform("blur_direction", sf::Glsl::Vec2(1.0, 0.0));
        blur_render.draw(sf::Sprite(blur_render.getTexture()), shader_states);
        blur_render.display();

        blur_shader.setUniform("blur_direction", sf::Glsl::Vec2(0.0, 1.0));
        blur_render.draw(sf::Sprite(blur_render.getTexture()), shader_states);
        blur_render.display();
    }

    shader_states.shader = &assemble_shader;
    assemble_shader.setUniform("sum", 2.0f);
    assemble_shader.setUniform("add_weight", 1.3f);
    assemble_shader.setUniform("add_texture", blur_render.getTexture());
    sf::Sprite tmp = sf::Sprite(scene_render.getTexture());
    scene_render.clear(sf::Color(0, 0, 0, 0));
    scene_render.draw(tmp, shader_states);
    scene_render.display();

    target.draw(sf::Sprite(scene_render.getTexture()), sf::BlendAdd);
}

void PostProcessing::add(sf::Drawable& drawable) {
     scene_render.draw(drawable);
}

void PostProcessing::clear() {
     scene_render.clear(sf::Color(0, 0, 0, 0));
}

void PostProcessing::create(const sf::Vector2f& size) {
     scene_render.create(size.x, size.y, ctx);
     luminescence_render.create(size.x, size.y, ctx);
     blur_render.create(size.x, size.y, ctx);
}
