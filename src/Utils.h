#ifndef XYLON_UTILS_H
#define XYLON_UTILS_H

#include <SFML/Graphics/Font.hpp>
#include <SFML/System/Clock.hpp>
#include "Settings.h"

extern sf::Font FONT, BOLD_FONT;
extern sf::Clock clk;
extern Settings settings;

extern void loadFonts();

#endif  // XYLON_UTILS_H
