#ifndef XYLON_SELECTVIEW_H
#define XYLON_SELECTVIEW_H

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <string>
#include <vector>
#include "../Utils.h"

class SelectView : public sf::Drawable {
public:
    SelectView(std::initializer_list<std::string>);
    ~SelectView() override;
    bool show = false;
    int ptr = 0;
    void up();
    void down();
    std::vector<std::string> items;
protected:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
};


#endif //XYLON_SELECTVIEW_H
