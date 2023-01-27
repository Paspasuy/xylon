#include "Utils.h"

#include <iostream>

#include "../gui/DownloadView.h"

sf::Font FONT, BOLD_FONT;
sf::Clock clk;
Settings settings;
ThreadPool threadPool;

void xylonInit() {
    std::locale::global(std::locale(settings.locale));
    std::wcin.imbue(std::locale(settings.locale));
    std::wcout.imbue(std::locale(settings.locale));

    DownloadView::last = clk.getElapsedTime() - sf::seconds(60);

    threadPool.start();
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

int clamp(int val, int low, int high) {
    return std::min(std::max(val, low), high);
}


std::wstring lower(const std::wstring& s) {
    std::wstring result(s);
    std::locale loc = std::wcin.getloc();
    for (wchar_t &it : result) {
        it = std::use_facet<std::ctype<wchar_t>>(loc).tolower(it);
    }
    return result;
}

std::string lower(const std::string& s) {
    std::string result(s);
    for (char &it : result) {
        it = tolower(it);
    }
    return result;
}

bool hasSubstr(const std::wstring& item, const std::wstring& str) {
    return lower(item).find(lower(str)) != std::wstring::npos;
}

bool hasSubstr(const std::string& item, const std::string& str) {
    return lower(item).find(lower(str)) != std::string::npos;
}

void dbgTime(const std::string& what, sf::Time start, sf::Time threshold, bool millis) {
    sf::Time t = clk.getElapsedTime() - start;
    if (t >= threshold) {
        std::cerr << "Time to render " << what << ": ";
        if (millis) {
            std::cerr << t.asMilliseconds()<< "ms" << std::endl;
        } else {
            std::cerr << t.asMicroseconds() << "μs" << std::endl;
        }
    }
}
