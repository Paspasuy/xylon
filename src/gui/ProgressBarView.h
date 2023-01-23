#ifndef XYLON_PROGRESSBARVIEW_H
#define XYLON_PROGRESSBARVIEW_H

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include "../Player.h"
#include "../Utils.h"

class ProgressBarView : public sf::Drawable {
private:
    int x, y;
    int current, duration;
    double progress;
    mutable sf::RectangleShape cont;

public:

    bool holding = false;

    void update(Player* pl);

    void setPosition(int x, int y);

    bool in_bar(int x, int y);

    void set_progress(Player* p, int x);

    ~ProgressBarView() override;

    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
};

#endif  // XYLON_PROGRESSBARVIEW_H
