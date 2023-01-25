#ifndef XYLON_UTILS_H
#define XYLON_UTILS_H

#include <SFML/Graphics/Font.hpp>
#include <SFML/System/Clock.hpp>

#include "Settings.h"

extern sf::Font FONT, BOLD_FONT;
extern sf::Clock clk;
extern Settings settings;

extern void xylonInit();
extern std::string secondsToTimeString(int time);
extern int clamp(int val, int low, int high);
extern bool hasSubstr(const std::wstring& item, const std::wstring& str);
extern bool hasSubstr(const std::string& item, const std::string& str);
std::wstring lower(const std::wstring& s);
std::string lower(const std::string& s);

#endif  // XYLON_UTILS_H
