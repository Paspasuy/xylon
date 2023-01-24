#include "Utils.h"

#include <iostream>

#include "../gui/DownloadView.h"

sf::Font FONT, BOLD_FONT;
sf::Clock clk;
Settings settings;

void xylonInit() {
    std::locale::global(std::locale(settings.locale));
    std::wcin.imbue(std::locale(settings.locale));
    std::wcout.imbue(std::locale(settings.locale));

    if (!FONT.loadFromFile(settings.pathToRegularFont)) {
        std::cerr << "FONTS BROKEN\n";
    }
    if (!BOLD_FONT.loadFromFile(settings.pathToBoldFont)) {
        std::cerr << "FONTS BROKEN\n";
    }

    DownloadView::last = clk.getElapsedTime() - sf::seconds(60);
}

std::string secondsToTimeString(int time) {
    std::string res;
    if (time / 60 < 10) {
        res += '0';
    }
    res += std::to_string(time / 60);
    res += ':';
    if (time % 60 < 10) {
        res += '0';
    }
    res += std::to_string(time % 60);
    return res;
}
