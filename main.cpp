#include <iostream>
#include "Player.cpp"
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "SongView.cpp"
#include "bass.h"

int main() {
    BASS_Init(1, 44100, 0, 0, NULL);
    auto *clock = new sf::Clock();
    auto *p = new Player(clock);
    p->add_folder("/home/pavel/Music/");
//    p -> add_song("amogus2.wav");
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode(800, 600), "xylon", sf::Style::Default, settings);
    window.setVerticalSyncEnabled(true);
    SongView songs;
    songs.init(p);
    p->play();
    sf::Font font;
    if (!font.loadFromFile("/usr/share/fonts/adobe-source-code-pro/SourceCodePro-Regular.otf")) {
        std::cerr << "FONTS BROKEN\n";
    }
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::MouseWheelScrolled) {
                if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
                    if (songs.get_click_index(event.mouseWheelScroll.x, event.mouseWheelScroll.y) != -1) {
                        songs.scroll(event.mouseWheelScroll.delta);
                    } else {
                        if (event.mouseWheelScroll.delta == 1) {
                            p -> add_vol();
                        } else {
                            p -> dec_vol();
                        }
                    }
                }
            }
            if (event.type == sf::Event::MouseButtonPressed) {
                int idx = songs.get_click_index(event.mouseButton.x, event.mouseButton.y);
                if (idx != -1) {
                    p->play_ind(idx);
                }
            }
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Space) {
                    if (p->is_playing()) {
                        p->pause();
                    } else {
                        p->play();
                    }
                }
                int idx = songs.get_click_index(event.mouseButton.x, event.mouseButton.y);
                if (idx != -1) {
                    p->play_ind(idx);
                }
            }
        }
        if (clock->getElapsedTime() > p->expire) {
            p->next();
        }
        window.clear();
        songs.render(window, font);
        window.display();
    }

    return 0;
}
