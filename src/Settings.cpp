//
// Created by pavel on 11/19/21.
//

#include "Settings.h"

void Settings::init() {
    std::ifstream conf;
    conf.open("conf.txt");
    std::string line;
    if (conf.is_open()) {
        conf >> vis_type;
    }
    conf.close();
}
