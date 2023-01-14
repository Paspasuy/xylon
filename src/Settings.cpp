//
// Created by pavel on 11/19/21.
//

#include <sstream>
#include "Settings.h"
#include <iostream>
#include <filesystem>

std::wstring DEFAULT_CONF_STR = L"# Visualiser type\n"
                                     "1\n"
                                     "# Usual tile color\n"
                                     "200 100 0 0\n"
                                     "200 100 0 30\n"
                                     "# Playing tile color\n"
                                     "0 0 200 0\n"
                                     "0 0 200 50\n"
                                     "# Progress bar color\n"
                                     "0 200 0 225\n"
                                     "# Visualiser color\n"
                                     "109 0 133 255\n"
                                     "# Repeat indicator color\n"
                                     "255 0 0 255\n"
                                     "# Folders\n";

void Settings::load() {
    std::wifstream conf;
    std::string str = getenv("HOME");
    str += "/.config/xylon/conf.txt";
    conf.open(str);
    if (!conf.is_open()) {
        std::filesystem::path path{ str };
        std::filesystem::create_directories(path.parent_path());
        std::wofstream cfg;
        cfg.open(str);
        cfg << DEFAULT_CONF_STR;
        cfg.close();
        conf.open(str);
//        std::filesystem::copy("/usr/share/xylon/sample_conf.txt", str);
//        conf.open(str + "/.config/xylon/conf.txt");
    }
    std::wstring line;
    int colors[28];
    int* params[29];
    params[0] = &vis_type;
    for (size_t i = 0; i < 28; ++i) {
        params[i + 1] = colors + i;
    }
    int i = 0, tmp = 0;
    int line_ind = 0;
    if (conf.is_open()) {
        while (getline(conf, line)) {
            int idx = line.find('#');
            if (idx != std::string::npos) {
                line = line.substr(0, idx);
            }
            idx = line.find('/');
            if (idx == std::string::npos) {
                std::wstringstream ss(line);
                while (ss >> tmp) {
                    *params[i++] = tmp;
                }
                ++line_ind;
            } else {
                std::string str(line.begin(), line.end());
                folders.push_back(str);
            }
        }
    }
    if (folders.empty()) {
        folders.push_back(std::string(getenv("HOME")) + "/Music/");
    }
    init_col(&c1, colors);
    init_col(&c2, colors + 4);
    init_col(&c3, colors + 8);
    init_col(&c4, colors + 12);
    init_col(&c5, colors + 16);
    init_col(&c6, colors + 20);
    init_col(&c7, colors + 24);
    conf.close();
}

void Settings::init_col(sf::Color *c, const int *colors) {
    c->r = colors[0];
    c->g = colors[1];
    c->b = colors[2];
    c->a = colors[3];
//    std::cerr << int(c->r) << ' '<< int(c->g) << ' '<< int(c->b) << ' '<< int(c->a) << '\n';
}

