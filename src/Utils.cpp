#include "Utils.h"

#include <iostream>

sf::Font FONT, BOLD_FONT;
sf::Clock clk;

void loadFonts() {
    if (!FONT.loadFromFile("/usr/share/fonts/adobe-source-code-pro/SourceCodePro-Regular.otf")) {
        std::cerr << "FONTS BROKEN\n";
    }

    if (!BOLD_FONT.loadFromFile("/usr/share/fonts/adobe-source-code-pro/SourceCodePro-Bold.otf")) {
        std::cerr << "FONTS BROKEN\n";
    }
}
