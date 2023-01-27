#include "ProgressBarView.h"

#include "SongDisplay.h"

void ProgressBarView::update(Player* pl) {
    sf::Time t1 = pl->getPlayingOffset();
    sf::Time t2 = pl->getDuration();
    int new_current = int(t1.asSeconds());
    int new_duration = int(t2.asSeconds());
    if (new_current != current) {
        current = new_current;
        currentText = sf::Text(secondsToTimeString(current), FONT, 14);
    }
    if (new_duration != duration) {
        duration = new_duration;
        std::string s2 = secondsToTimeString(duration);
        durationText = sf::Text(s2, FONT, 14);
    }
    progress = double(t1.asMicroseconds()) / t2.asMicroseconds();
}

ProgressBarView::~ProgressBarView() {}

void ProgressBarView::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    sf::RectangleShape line(sf::Vector2f(SongDisplay::PIC, 3.f));
    line.setPosition(x, y + 5);
    line.setFillColor(sf::Color(127, 127, 127));
    target.draw(line);
    cont = line;
    line.setSize(sf::Vector2f(int(SongDisplay::PIC * progress), 3.f));
    line.setPosition(x, y + 5);
    line.setFillColor(settings.progressBarColor);
    target.draw(line);

    currentText.setPosition(x, y + 10);
    target.draw(currentText);

//    std::string s2 = secondsToTimeString(duration);
    durationText.setPosition(x + SongDisplay::PIC - 8 * 5, y + 10);
    target.draw(durationText);
}

void ProgressBarView::setPosition(int _x, int _y) {
    x = _x;
    y = _y;
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

void ProgressBarView::set_progress(Player* p, int x) {
    x -= cont.getPosition().x;
    x = std::max(x, 0);
    x = std::min(x, int(cont.getSize().x));
    progress = double(x) / cont.getSize().x;
    p->set_position(progress);
}
