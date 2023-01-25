#ifndef XYLON_DIRECTORYSELECT_H
#define XYLON_DIRECTORYSELECT_H

#include "../Player.h"
#include "../utils/DirectoryNode.h"
#include "SelectView.h"

class DirectorySelect : public SelectView {
public:
    std::string currentPath;
    DirectorySelect();
    Player player;
    void loadToPlayer();
    void init();
    void filter(const std::string& str) override;
    void filter(const std::wstring& str) override;

    DirectoryNode root;
};

#endif  // XYLON_DIRECTORYSELECT_H
