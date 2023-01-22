#include "ProgressBarView.h"

#include "SongDisplay.h"

void ProgressBarView::update(Player* pl) {
    sf::Time t1 = pl->getPlayingOffset();
    sf::Time t2 = pl->getDuration();
    current = int(t1.asSeconds());
    duration = int(t2.asSeconds());
    progress = double(t1.asMicroseconds()) / t2.asMicroseconds();
}

void ProgressBarView::render(sf::RenderWindow& window, sf::Font& font, int x, int y) {
    sf::RectangleShape line(sf::Vector2f(SongDisplay::PIC, 3.f));
    line.setPosition(x, y + 5);
    line.setFillColor(sf::Color(127, 127, 127));
    window.draw(line);
    cont = line;
    line.setSize(sf::Vector2f(int(SongDisplay::PIC * progress), 3.f));
    line.setPosition(x, y + 5);
    line.setFillColor(s->c5);
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
    text2.setPosition(x + SongDisplay::PIC - 8 * s2.size(), y + 10);
    window.draw(text2);
}

bool ProgressBarView::in_bar(int x, int y) {
    int x1 = cont.getPosition().x;
    int y1 = cont.getPosition().y - 20;
    int x2 = cont.getPosition().x + cont.getSize().x;
    int y2 = cont.getPosition().y + cont.getSize().y + 20;
    if (x1 <= x && x2 >= x) {
        if (y1 <= y && y2 >= y) {
            return true;
        }
    }
    return false;
}

void ProgressBarView::set_position(Player* p, int x) {
    x -= cont.getPosition().x;
    x = std::max(x, 0);
    x = std::min(x, int(cont.getSize().x));
    progress = double(x) / cont.getSize().x;
    p->set_position(progress);
}

ProgressBarView::ProgressBarView(Settings* _s) { s = _s; }
