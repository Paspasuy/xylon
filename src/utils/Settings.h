//
// Created by pavel on 11/19/21.
//

#ifndef XYLON_SETTINGS_H
#define XYLON_SETTINGS_H

#include <SFML/Graphics/Color.hpp>
#include <fstream>
#include <vector>

class Settings {
    std::string path;

public:
    Settings();
    std::vector<std::string> folders;
    int vis_type = 0;
    sf::Color c1, c2, c3, c4, c5, c6, c7;
    static void init_col(sf::Color* c, const int* colors);
    void load();
};

#endif  // XYLON_SETTINGS_H
