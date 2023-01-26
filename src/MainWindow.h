#ifndef XYLON_MAINWINDOW_H
#define XYLON_MAINWINDOW_H

#include <SFML/Graphics/RenderWindow.hpp>

#include "../SelbaWard/Starfield.h"
#include "Player.h"
#include "SongList.h"
#include "gui/DirectorySelect.h"
#include "gui/DownloadView.h"
#include "gui/MouseTrace.h"
#include "gui/SongDisplay.h"
#include "gui/SongSearch.h"
#include "gui/SortSelect.h"
#include "gui/Visualiser.h"
#include "gui/VolumeCircleSlider.h"
#include "shaders/PostProcessing.h"
#include "utils/PicLoader.h"
#include "utils/Settings.h"
#include "utils/Utils.h"

class MainWindow : public sf::RenderWindow {
    sw::Starfield starfield;
    DirectorySelect dirSelect;
    Player& p;
    SongDisplay songDisplay;
    VolumeCircleSlider vol_slider;
    SongList songList;
    PicLoader picLoader;
    SongSearch songSearch;
    Visualiser visualiser;
    SortSelect sortSelect;
    sf::Vector2f stars_vec;
    DownloadView download;
    MouseTrace mouseTrace;
    PostProcessing postProcessing;

    bool processGeneralEvent(sf::Event& event);
    bool processMouseEvent(sf::Event& event);

public:
    explicit MainWindow(sf::ContextSettings contextSettings);
    void pollEvents();
    void beforeRender(uint64_t frame);
    void render();
    void beforePolling();
};

#endif  // XYLON_MAINWINDOW_H
