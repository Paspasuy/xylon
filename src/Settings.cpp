//
// Created by pavel on 11/19/21.
//

#include <sstream>
#include "Settings.h"

void Settings::init() {
    std::wifstream conf;
    conf.open("conf.txt");
    std::wstring line;
    int colors[24];
    int *params[25];
    params[0] = &vis_type;
    for (size_t i = 0; i < 24; ++i) {
        params[i + 1] = colors + i;
    }
    int i = 0;
    int line_ind = 0;
    if (conf.is_open()) {
        while (getline(conf, line)) {
            int idx = line.find('#');
            if (idx != std::string::npos) {
                line = line.substr(0, idx);
            }
            std::wstringstream ss(line);
            while (ss >> *params[i]) {
                ++i;
            }
            ++line_ind;
        }
    }
    init_col(&c1, colors);
    init_col(&c2, colors + 4);
    init_col(&c3, colors + 8);
    init_col(&c4, colors + 12);
    init_col(&c5, colors + 16);
    init_col(&c6, colors + 20);
    conf.close();
}

void Settings::init_col(sf::Color *c, const int *colors) {
    c->r = colors[0];
    c->g = colors[1];
    c->b = colors[2];
    c->a = colors[3];
//    std::cerr << int(c->r) << ' '<< int(c->g) << ' '<< int(c->b) << ' '<< int(c->a) << '\n';
}
