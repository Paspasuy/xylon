#include "MainWindow.h"
#include "bass.h"
#include "utils/Utils.h"

int main(int argc, char** argv) {
    srand(time(0));
    std::locale::global(std::locale("ru_RU.utf-8"));
    std::wcin.imbue(std::locale("ru_RU.utf-8"));
    std::wcout.imbue(std::locale("ru_RU.utf-8"));
    BASS_Init(1, 44100, 0, 0, NULL);

    xylonInit();

    sf::ContextSettings contextSettings;
    contextSettings.antialiasingLevel = 8;
    MainWindow window(contextSettings);

    for (uint64_t frame = 0; window.isOpen(); ++frame) {
        window.beforePolling();
        window.pollEvents();
        window.beforeRender(frame);
        window.render();
    }

    return 0;
}

// fix all bugs in songs list
// shaders
// somehow compile for windows (in distant future)
// write help menu, write readme
// add different file formats support (flac?)
