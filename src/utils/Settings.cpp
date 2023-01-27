#include "Settings.h"

#include <filesystem>
#include <iostream>

#include "Utils.h"

std::wstring DEFAULT_CONF_STR =
    L"# Locale\n"
    "en_US.utf-8\n"
    "# Path to regular font\n"
    "/usr/share/fonts/adobe-source-code-pro/SourceCodePro-Regular.otf\n"
    "# Path to bold font\n"
    "/usr/share/fonts/adobe-source-code-pro/SourceCodePro-Bold.otf\n"
    "# Visualiser type (0/1)\n"
    "0\n"
    "# Framerate limit\n"
    "120\n"
    "# Enable glow\n"
    "1\n"
    "# Inactive tile color (outline/background), all colors are in 'r g b a' format\n"
    "200 100 0 0\n"
    "200 100 0 30\n"
    "# Active tile color (outline/background)\n"
    "0 0 200 0\n"
    "0 0 200 50\n"
    "# Progress bar color\n"
    "0 200 0 225\n"
    "# Visualiser color\n"
    "160 0 190 255\n"
    "# Repeat indicator color\n"
    "255 0 0 255\n"
    "# Cursor color\n"
    "255 255 0 255\n"
    "# Cursor trace color\n"
    "255 255 0 255\n"
    "# Folders\n";

void Settings::load() {
    pathToRegularFont.clear();
    pathToBoldFont.clear();
    locale.clear();

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

    std::string line;

    if (conf.is_open()) {
        locale = readString(conf);
        pathToRegularFont = readString(conf);
        pathToBoldFont = readString(conf);
        vis_type = readInt(conf);
        framerateLimit = readInt(conf);
        enableGlow = readInt(conf);
        inactiveTileOutline = readColor(conf);
        inactiveTileBackground = readColor(conf);
        activeTileOutline = readColor(conf);
        activeTileBackground = readColor(conf);
        progressBarColor = readColor(conf);
        visualiserColor = readColor(conf);
        repeatColor = readColor(conf);
        cursorColor = readColor(conf);
        cursorTraceColor = readColor(conf);
        while (!(line = readString(conf)).empty()) {
            folders.emplace_back(line);
        }
    }
    conf.close();
    if (folders.empty()) {
        folders.emplace_back(std::string(getenv("HOME")) + "/Music/");
    }
    loadFonts();
}

Settings::Settings() { load(); }

void Settings::loadFonts() {
    sf::Font font, bold_font;
    if (!font.loadFromFile(settings.pathToRegularFont)) {
        std::wcerr << "font broken\n";
    } else {
        FONT = font;
    }
    if (!bold_font.loadFromFile(settings.pathToBoldFont)) {
        std::wcerr << "bold font broken\n";
    } else {
        BOLD_FONT = bold_font;
    }
}

void removeComment(std::wstring& line) {
    int idx = line.find('#');
    if (idx != std::string::npos) {
        line = line.substr(0, idx);
    }
}

int Settings::readInt(std::wifstream& file) {
    std::wstring line;
    while (getline(file, line)) {
        removeComment(line);
        if (line.empty()) continue;
        int x;
        std::wstringstream ss(line);
        ss >> x;
        return x;
    }
    throw std::runtime_error("Reading integer from config failed");
}

sf::Color Settings::readColor(std::wifstream& file) {
    std::wstring line;
    while (getline(file, line)) {
        removeComment(line);
        if (line.empty()) continue;
        int r, g, b, a;
        std::wstringstream ss(line);
        ss >> r >> g >> b >> a;
        return sf::Color(r, g, b, a);
    }
    throw std::runtime_error("Reading color from config failed");
}

std::wstring Settings::readWstring(std::wifstream& file) {
    std::wstring line;
    while (getline(file, line)) {
        removeComment(line);
        if (line.empty()) continue;
        return line;
    }
    return L"";
}

std::string Settings::readString(std::wifstream& file) {
    std::wstring res = readWstring(file);
    return std::string(res.begin(), res.end());
}
