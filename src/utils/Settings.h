#ifndef XYLON_SETTINGS_H
#define XYLON_SETTINGS_H

#include <SFML/Graphics/Color.hpp>
#include <fstream>
#include <vector>

class Settings {
    std::string path;

    int readInt(std::wifstream& file);
    sf::Color readColor(std::wifstream& file);
    std::wstring readWstring(std::wifstream& file);
    std::string readString(std::wifstream& file);

public:
    Settings();
    std::string locale;
    std::string pathToRegularFont;
    std::string pathToBoldFont;
    std::vector<std::string> folders;
    int vis_type;
    int framerateLimit;
    int enableGlow;
    sf::Color inactiveTileOutline, inactiveTileBackground;
    sf::Color activeTileOutline, activeTileBackground;
    sf::Color progressBarColor, visualiserColor, repeatColor;
    sf::Color cursorColor, cursorTraceColor;

    void load();

    void loadFonts();
};

#endif  // XYLON_SETTINGS_H
