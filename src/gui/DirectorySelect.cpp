#include "DirectorySelect.h"

std::string ltrim(const std::string& s) {
    return std::string(
        std::find_if(s.begin(), s.end(), [](unsigned char ch) { return !std::isspace(ch); }),
        s.end());
}

DirectorySelect::DirectorySelect() : SelectView(), root("") {}

void DirectorySelect::loadToPlayer() {
    player.reset();
    currentPath = ltrim(items[ptr]);
    player.add_folder(currentPath);
    if (player.empty()) {
        throw std::runtime_error("No songs in folder");
    }
    player.sort_by_date();
    player.ptr = 0;
}

void DirectorySelect::init() {
    root.collectNames(items);
    loadToPlayer();
}
