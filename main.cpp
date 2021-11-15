#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "bass.h"
#include "Player.h"
#include "SongView.h"
#include "SongSearch.h"
#include "SongDisplay.h"

int main() {
    std::locale::global(std::locale("ru_RU.utf-8"));
    std::wcin.imbue(std::locale("ru_RU.utf-8"));
    std::wcout.imbue(std::locale("ru_RU.utf-8"));

    BASS_Init(1, 44100, 0, 0, NULL);
    auto *clock = new sf::Clock();
    auto *p = new Player(clock);
    p->add_folder("/home/pavel/Music/");
    for (auto &it: p->songs) {
        it->add_meta();
    }
    auto *cpl = p;
    auto *display = new SongDisplay(p);
    std::sort(p->songs.begin(), p->songs.end(), [&](Song *i, Song *j) {
        return std::make_pair(i->album, i->artist) < std::make_pair(j->album, j->artist);
    });
//    p -> add_song("/home/pavel/Music/amogus2.wav");
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode(1024, 768), "xylon", sf::Style::Default, settings);
    Tile::W = std::min(int(window.getSize().x) / 2, MIN_W);
    window.setVerticalSyncEnabled(true);
    SongView songs;
    songs.init(p);
    p->play();
    sf::Font font;
    if (!font.loadFromFile("/usr/share/fonts/adobe-source-code-pro/SourceCodePro-Regular.otf")) {
        std::cerr << "FONTS BROKEN\n";
    }
    sf::Font bold_font;
    if (!bold_font.loadFromFile("/usr/share/fonts/adobe-source-code-pro/SourceCodePro-Bold.otf")) {
        std::cerr << "FONTS BROKEN\n";
    }
    auto songSearch = new SongSearch(p);
    while (window.isOpen()) {
        if (clock->getElapsedTime() > p->expire && p->is_playing()) {
            p->next();
        }
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                return 0;
            }
            if (event.type == sf::Event::Resized) {
                Tile::W = std::min(int(window.getSize().x) / 2, MIN_W);
                sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
                window.setView(sf::View(visibleArea));
                songs.winsz = window.getSize();
            } else if (event.type == sf::Event::MouseWheelScrolled) {
                if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
                    if (songs.get_click_id(event.mouseWheelScroll.x, event.mouseWheelScroll.y).first != -1) {
                        int delta = event.mouseWheelScroll.delta;
                        if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) {
                            delta *= 5;
                        }
                        songs.scroll(delta);
                    } else {
                        if (event.mouseWheelScroll.delta == 1) {
                            p->add_vol();
                        } else {
                            p->dec_vol();
                        }
                    }
                }
            } else if (event.type == sf::Event::MouseButtonPressed) {
                auto [id, idx] = songs.get_click_id(event.mouseButton.x, event.mouseButton.y);
                if (id != -1) {
                    if (songSearch->state()) {
                        cpl->set_index(idx);
                    }
                    p->play_id(id);
                } else if (display->bar.in_bar(event.mouseButton.x, event.mouseButton.y)) {
                    display->bar.holding = 1;
                    p->pause();
                    display->bar.set_position(p, event.mouseButton.x);
                }
            } else if (event.type == sf::Event::MouseMoved) {
                if (display->bar.holding) {
                    std::cerr << event.mouseMove.x << std::endl;
                    display->bar.set_position(p, event.mouseMove.x);
                }
            } else if (event.type == sf::Event::MouseButtonReleased) {
                if (display->bar.holding) {
                    display->bar.set_position(p, event.mouseButton.x);
                    display->bar.holding = 0;
                    p->play();
                }
            } else if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Space) {
                    if (!songSearch->state()) {
                        if (p->is_playing()) {
                            p->pause();
                        } else {
                            p->play();
                        }
                    }
                } else if (event.key.code == sf::Keyboard::Left) {
                    p->backward_5();
                } else if (event.key.code == sf::Keyboard::Right) {
                    p->forward_5();
                } else if (event.key.code == sf::Keyboard::Up) {
                    p->prev();
                    songs.norm_shift_up();
                } else if (event.key.code == sf::Keyboard::Down) {
                    p->next();
                    songs.norm_shift_down();
                } else if (event.key.code == sf::Keyboard::R && event.key.control) {
                    p->loop ^= 1;
                } else if (event.key.code == sf::Keyboard::Escape) {
                    if (songSearch->state()) {
                        cpl = songSearch->clear();
                        songs.init(cpl);
                    }
                } else if (event.key.code == sf::Keyboard::BackSpace) {
                    if (songSearch->state()) {
                        if (event.key.control) {
                            cpl = songSearch->pop_word();
                            songs.init(cpl);
                        } else {
                            cpl = songSearch->pop_char();
                            songs.init(cpl);
                        }
                    }
                } else if (event.key.code == sf::Keyboard::Q && event.key.control) {
                    window.close();
                    return 0;
                }
            } else if (event.type == sf::Event::TextEntered) {
                if (event.text.unicode != 27 && event.text.unicode != 18 && event.text.unicode != 8 &&
                    event.text.unicode != 13 && event.text.unicode != 26) {
                    std::wcout << L"ASCII character typed: " << static_cast<wchar_t>(event.text.unicode) << ' '
                              << event.text.unicode << std::endl;
                    if (event.text.unicode != 32 || songSearch->state()) {
                        cpl = songSearch->add_char(event.text.unicode);
                        songs.init(cpl);
                    }
                }
            }
        }
        window.clear();
        songs.render(window, font, bold_font);
        songSearch->render(window, font);
        display->render(window, font, bold_font);
        window.display();
    }

    return 0;
}

// TODO: albums support
// TODO: maybe add second margin (Oy)
// TODO: inertial scrolling
// TODO: add focused song (not current!)
// TODO: album cover from real song
// TODO: create new class for current song displaying
// TODO: add github token to clion/git/whatever
// TODO: add volume circle
// done: add background for search
// TODO: add visualiser
// TODO: settings in config file
// download songs from internet
// TODO: add icon for repeating
