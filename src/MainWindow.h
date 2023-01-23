#ifndef XYLON_MAINWINDOW_H
#define XYLON_MAINWINDOW_H

#include <SFML/Graphics/RenderWindow.hpp>

#include "../SelbaWard/Starfield.h"
#include "PicLoader.h"
#include "Player.h"
#include "Settings.h"
#include "SongDisplay.h"
#include "SongSearch.h"
#include "SongView.h"
#include "SortSelect.h"
#include "Utils.h"
#include "Visualiser.h"
#include "VolumeCircleSlider.h"

class MainWindow : public sf::RenderWindow {
    Settings settings;
    sw::Starfield starfield;
    Player p;
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
