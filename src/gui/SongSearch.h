#ifndef XYLON_SONGSEARCH_H
#define XYLON_SONGSEARCH_H

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include "../Player.h"
#include "../utils/Utils.h"

class SongSearch : public sf::Drawable {
public:
    std::wstring text;
    sf::Color back_col = sf::Color(32, 32, 32);

    void add_char(wchar_t c);

    void pop_char();

    void pop_word();

    void clear();

    bool empty();

    const std::wstring& get_filter();

    void update_color(size_t i);

    ~SongSearch() override;

protected:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

#endif  // XYLON_SONGSEARCH_H
