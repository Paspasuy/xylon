//
// Created by pavel on 11/25/21.
//

#ifndef XYLON_SORTSELECT_H
#define XYLON_SORTSELECT_H


#include <string>
#include <vector>
#include <SFML/Graphics/RenderWindow.hpp>
#include "Player.h"

class SortSelect {
public:
    std::vector<std::string> sorts{"Date", "Album", "Random"};
    bool state = false;
    int ptr = 0;
    void up();
    void down();
    void render(sf::RenderWindow &window, sf::Font font);
    void sort(Player *p);

};


#endif //XYLON_SORTSELECT_H
