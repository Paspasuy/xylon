#ifndef XYLON_MAINWINDOW_H
#define XYLON_MAINWINDOW_H

#include <SFML/Graphics/RenderWindow.hpp>

#include "../SelbaWard/Starfield.h"
#include "gui/DirectorySelect.h"
#include "utils/PicLoader.h"
#include "Player.h"
#include "utils/Settings.h"
#include "gui/SongDisplay.h"
#include "gui/SongSearch.h"
#include "SongView.h"
#include "gui/SortSelect.h"
#include "utils/Utils.h"
#include "gui/Visualiser.h"
#include "gui/VolumeCircleSlider.h"

class MainWindow : public sf::RenderWindow {
    sw::Starfield starfield;
    DirectorySelect dirSelect;
    Player& p;
    SongDisplay songDisplay;
    VolumeCircleSlider vol_slider;
    SongView songs;
    PicLoader pl;
    SongSearch songSearch;
    Visualiser visualiser;
    SortSelect sortSelect;
    sf::Vector2f stars_vec;

public:
    MainWindow(sf::ContextSettings contextSettings);
    void pollEvents();
    void beforeRender(uint64_t frame);
    void render();
    void beforePolling();
};

#endif  // XYLON_MAINWINDOW_H
