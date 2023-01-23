#ifndef XYLON_SORTSELECT_H
#define XYLON_SORTSELECT_H

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <string>
#include <vector>

#include "../Player.h"
#include "SelectView.h"

class SortSelect : public SelectView {
public:
    SortSelect();
    void applySort(Player* p);
};

#endif  // XYLON_SORTSELECT_H
