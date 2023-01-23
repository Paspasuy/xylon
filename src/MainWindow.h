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
#include "Visualiser.h"
#include "VolumeCircleSlider.h"

class MainWindow : public sf::RenderWindow {
    Settings settings;
    sw::Starfield starfield;
    sf::Clock clock;
    Player p;
    SongDisplay songDisplay;
    VolumeCircleSlider vol_slider;
    sf::Font font, bold_font;
    SongView songs;
    PicLoader pl;
    SongSearch songSearch;
    Visualiser visualiser;
    SortSelect sortSelect;
    float fft[2048];
    sf::Vector2f stars_vec;

public:
    MainWindow(sf::ContextSettings ctxsettings);
    void pollEvents();
    void beforeRender(uint64_t frame);
    void render();
    void beforePolling();
};

#endif  // XYLON_MAINWINDOW_H
