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

    DirectoryNode root;
};

#endif  // XYLON_DIRECTORYSELECT_H
