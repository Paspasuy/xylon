#ifndef XYLON_DIRECTORYSELECT_H
#define XYLON_DIRECTORYSELECT_H

#include "DirectoryNode.h"
#include "Player.h"
#include "gui/SelectView.h"

class DirectorySelect : public SelectView {
public:
    Player player;
    DirectorySelect();
    void loadToPlayer();
    void init();

    DirectoryNode root;
};

#endif  // XYLON_DIRECTORYSELECT_H
