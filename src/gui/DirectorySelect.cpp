#include "DirectorySelect.h"

std::string ltrim(const std::string& s) {
    return std::string(
        std::find_if(s.begin(), s.end(), [](unsigned char ch) { return !std::isspace(ch); }),
        s.end());
}

DirectorySelect::DirectorySelect() : SelectView(), root("") {}

void DirectorySelect::loadToPlayer() {
    player.reset();
    currentPath =
        ltrim(std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(visibleItems[ptr]));
    player.add_folder(currentPath);
    if (player.empty()) {
        throw std::runtime_error("No songs in folder");
    }
    player.sort_by_date();
    player.ptr = 0;
}

void DirectorySelect::init() {
    root.collectNames(items);
    visibleItems = items;
    loadToPlayer();
}

void DirectorySelect::filter(const std::string& str) {
    filter(std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(str));
}

void DirectorySelect::filter(const std::wstring& str) {
    ptr = 0;
    if (str.empty()) {
        visibleItems = items;
    } else {
        visibleItems.clear();
        for (std::wstring& item : items) {
            if (hasSubstr(std::wstring(item.rfind('/') + item.begin(), item.end()), str)) {
                visibleItems.emplace_back(item);
            }
        }
    }
}