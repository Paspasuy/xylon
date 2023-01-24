//
// Created by pavel on 11/19/21.
//

#include "Settings.h"

#include <filesystem>
#include <iostream>
#include <sstream>

std::wstring DEFAULT_CONF_STR =
    L"# Locale\n"
    "en_US.utf-8\n"
    "# Path to regular font\n"
    "/usr/share/fonts/adobe-source-code-pro/SourceCodePro-Regular.otf\n"
    "# Path to bold font\n"
    "/usr/share/fonts/adobe-source-code-pro/SourceCodePro-Bold.otf\n"
    "# Visualiser type (0/1)\n"
    "1\n"
    "# Usual tile color (outline/background), all colors are in 'r g b a' format\n"
    "200 100 0 0\n"
    "200 100 0 30\n"
    "# Playing tile color (outline/background)\n"
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
        std::filesystem::path path{str};
        std::filesystem::create_directories(path.parent_path());
        std::wofstream cfg;
        cfg.open(str);
        cfg << DEFAULT_CONF_STR;
        cfg.close();
        conf.open(str);
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
            if (line.empty()) continue;
            if (locale.empty()) {
                locale = std::string(line.begin(), line.end());
                continue;
            }
            if (pathToRegularFont.empty()) {
                pathToRegularFont = std::string(line.begin(), line.end());
                continue;
            }
            if (pathToBoldFont.empty()) {
                pathToBoldFont = std::string(line.begin(), line.end());
                continue;
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
        folders.emplace_back(std::string(getenv("HOME")) + "/Music/");
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

void Settings::init_col(sf::Color* c, const int* colors) {
    c->r = colors[0];
    c->g = colors[1];
    c->b = colors[2];
    c->a = colors[3];
}

Settings::Settings() { load(); }
