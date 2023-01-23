#ifndef XYLON_SELECTVIEW_H
#define XYLON_SELECTVIEW_H

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <string>
#include <vector>
#include "../Utils.h"

class SelectView : public sf::Drawable {
    const int PAGE = 10;
    const int H = 30;
public:
    SelectView();
    SelectView(std::initializer_list<std::string>);
    ~SelectView() override;
    bool show = false;
    int ptr = 0;
    void up();
    void down();
    void pageUp();
    void pageDown();
    std::vector<std::string> items;
protected:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
};


#endif //XYLON_SELECTVIEW_H
