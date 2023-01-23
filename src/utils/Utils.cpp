#include "Utils.h"
#include "../gui/DownloadView.h"

#include <iostream>

sf::Font FONT, BOLD_FONT;
sf::Clock clk;
Settings settings;

void xylonInit() {
    if (!FONT.loadFromFile("/usr/share/fonts/adobe-source-code-pro/SourceCodePro-Regular.otf")) {
        std::cerr << "FONTS BROKEN\n";
    }

    if (!BOLD_FONT.loadFromFile("/usr/share/fonts/adobe-source-code-pro/SourceCodePro-Bold.otf")) {
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
