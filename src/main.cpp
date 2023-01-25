#include "MainWindow.h"
#include "bass.h"
#include "utils/Utils.h"

int main(int argc, char** argv) {
    srand(time(0));
    xylonInit();
    BASS_Init(1, 44100, 0, 0, NULL);

    sf::ContextSettings contextSettings;
    contextSettings.antialiasingLevel = 8;
    MainWindow window(contextSettings);

    for (uint64_t frame = 0; window.isOpen(); ++frame) {
        sf::Time t = clk.getElapsedTime();
        window.beforePolling();
        window.pollEvents();
        window.beforeRender(frame);
        window.render();
//        std::cerr << "Rendered in " << (clk.getElapsedTime() - t).asMilliseconds() << "ms" << std::endl;
    }

    return 0;
}

// fix freezes when loading pictures
// button resizing with bass
// somehow compile for windows (in distant future)
// write help menu, write readme
// add different file formats support (flac?)
