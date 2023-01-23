#include "MainWindow.h"
#include "bass.h"

int main(int argc, char* argv[]) {
    srand(time(0));
    std::locale::global(std::locale("ru_RU.utf-8"));
    std::wcin.imbue(std::locale("ru_RU.utf-8"));
    std::wcout.imbue(std::locale("ru_RU.utf-8"));
    BASS_Init(1, 44100, 0, 0, NULL);

    sf::ContextSettings ctxsettings;
    ctxsettings.antialiasingLevel = 8;
    MainWindow window(ctxsettings);

    for (uint64_t frame = 0; window.isOpen(); ++frame) {
        window.beforePolling();
        window.pollEvents();
        window.beforeRender(frame);
        window.render();
    }

    return 0;
}

// TODO: folders support
// TODO: song duration in song list
// TODO: GUI for downloading songs from Internet (yt-dlp?)
// TODO: shaders
// TODO: somehow compile for windows (in distant future)
// TODO: write help menu, write readme
// TODO: add different file formats support (flac?)
