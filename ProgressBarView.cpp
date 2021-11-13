//#include "ProgressBarView.h"
#include "Player.cpp"
#include "Tile.cpp"
#include "SongView.cpp"

#ifndef XYLON_PROGRESSBARVIEW_H
#define XYLON_PROGRESSBARVIEW_H
class ProgressBarView {
public:
    double progress;
    const int height = 3;
    int width;
public:
    int current, duration;
    void update(Player *pl) {
        sf::Time t1 = pl->getPlayingOffset();
        sf::Time t2 = pl->getDuration();
        current = int(t1.asSeconds());
        duration = int(t2.asSeconds());
        progress = double(t1.asMicroseconds()) / t2.asMicroseconds();
    }
    void render(sf::RenderWindow &window, sf::Font &font, int x, int y) {
        sf::RectangleShape line(sf::Vector2f(Tile::PIC, 3.f));
        line.setPosition(x, y + 5);
        line.setFillColor(sf::Color(127, 127, 127));
        window.draw(line);
        line.setSize(sf::Vector2f(int(Tile::PIC * progress), 3.f));
        line.setPosition(x, y + 5);
        line.setFillColor(sf::Color::Green);
        window.draw(line);
        std::string s1;
        if (current / 60 < 10) {
            s1 += "0";
        }
        s1 += std::to_string(current / 60);
        s1 += ":";
        if (current % 60 < 10) {
            s1 += "0";
        }
        s1 += std::to_string(current % 60);
        std::string s2;
        if (duration / 60 < 10) {
            s2 += "0";
        }
        s2 += std::to_string(duration / 60);
        s2 += ":";
        if (duration % 60 < 10) {
            s2 += "0";
        }
        s2 += std::to_string(duration % 60);
        sf::Text text(s1, font, 14);
        text.setPosition(x, y + 10);
        window.draw(text);
        sf::Text text2(s2, font, 14);
        text2.setPosition(x + Tile::PIC - 8 * s2.size(), y + 10);
        window.draw(text2);


    }
};
#endif //XYLON_PROGRESSBARVIEW_H
