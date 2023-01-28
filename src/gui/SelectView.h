#ifndef XYLON_SELECTVIEW_H
#define XYLON_SELECTVIEW_H

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <string>
#include <vector>

#include "../utils/Utils.h"

class SelectView : public sf::Drawable {
    const int PAGE = 10;
    const int H = 30;
    mutable int shift = 0;

protected:
    std::vector<std::wstring> items;

public:
    bool show = false;
    int ptr = 0;
    mutable std::vector<std::wstring> visibleItems;

    SelectView();
    explicit SelectView(const std::vector<std::wstring>& items);
    SelectView(std::initializer_list<std::wstring>);
    ~SelectView() override;
    void init(const std::vector<std::wstring>&);

    void up();
    void down();
    void pageUp();
    void pageDown();
    virtual void filter(const std::string& str);
    virtual void filter(const std::wstring& str);
    size_t size();

protected:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

#endif  // XYLON_SELECTVIEW_H
