//
// Created by pavel on 11/25/21.
//

#include <fstream>
#include "FolderSelect.h"

FolderSelect::FolderSelect() {
    std::wifstream conf;
    conf.open("folders.txt");
    std::wstring line;
    if (conf.is_open()) {
        while (getline(conf, line)) {
            std::string str(line.begin(), line.end());
            folders.push_back(str);
        }
    } else {
        folders.push_back(std::string(getenv("HOME")) + "/Music/");
    }
}