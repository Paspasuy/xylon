#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "bass.h"
#include "Player.h"
#include "SongView.h"
#include "SongSearch.h"
#include "SongDisplay.h"
#include "VolumeCircleSlider.h"
#include "../SelbaWard/Starfield.h"
#include "Visualiser.h"
#include "Settings.h"
#include "SortSelect.h"

void stars_rot(sf::Vector2f &vec, float sin=0.001) {
    float x1 = vec.x;
    float y1 = vec.y;
    float cos = 1 - sin * sin;
    vec.x = cos * x1 - sin * y1;
    vec.y = sin * x1 + cos * y1;
}

int main(int argc, char *argv[]) {
    srand(time(0));
    int winw = 1024;
    int winh = 768;
    std::locale::global(std::locale("ru_RU.utf-8"));
    std::wcin.imbue(std::locale("ru_RU.utf-8"));
    std::wcout.imbue(std::locale("ru_RU.utf-8"));
    Settings settings;
    settings.load();
    BASS_Init(1, 44100, 0, 0, NULL);
    sf::Clock clock;
    Player p(&clock);
    for (std::string &s: settings.folders) {
        p.add_folder(s);
    }
    p.sort_by_date();
    p.ptr = 0;
    SongDisplay display(&p, &settings);
    VolumeCircleSlider vol_slider(&p, clock.getElapsedTime());
    sw::Starfield starfield(sf::Vector2f(winw, winh), 700);
    sf::ContextSettings ctxsettings;
    ctxsettings.antialiasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode(winw, winh), "xylon", sf::Style::Default, ctxsettings);
    Tile::W = std::min(int(window.getSize().x) / 2, Tile::MAX_W);
    window.setVerticalSyncEnabled(true);
    SongView songs(&settings);
    songs.init(&p);
    p.play();
    sf::Font font;
    if (!font.loadFromFile("/usr/share/fonts/adobe-source-code-pro/SourceCodePro-Regular.otf")) {
        std::cerr << "FONTS BROKEN\n";
    }
    sf::Font bold_font;
    if (!bold_font.loadFromFile("/usr/share/fonts/adobe-source-code-pro/SourceCodePro-Bold.otf")) {
        std::cerr << "FONTS BROKEN\n";
    }
    SongSearch songSearch;
    sf::Vector2f stars_vec(0.4, -0.1);
    stars_rot(stars_vec, (rand() % 100) / 100.f);
    Visualiser visualiser(&settings);
    SortSelect sortSelect;
    float fft[2048];
    while (window.isOpen()) {
        if (clock.getElapsedTime() > p.expire && p.is_playing()) {
            songs.play_next();
        }
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                return 0;
            }
            if (event.type == sf::Event::Resized) {
                Tile::W = std::min(int(window.getSize().x) / 2, Tile::MAX_W);
                sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
                window.setView(sf::View(visibleArea));
                songs.winsz = window.getSize();
                starfield.regenerate(sf::Vector2f(window.getSize().x, window.getSize().y));
            } else if (event.type == sf::Event::MouseWheelScrolled) {
                if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
                    int delta = event.mouseWheelScroll.delta;
                    if (songs.get_click_id(event.mouseWheelScroll.x, event.mouseWheelScroll.y).first != -1) {
                        if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) {
                            delta *= 5;
                        }
                        songs.scroll(delta);
                    } else {
                        vol_slider.touch(clock.getElapsedTime());
                        if (delta == 1) {
                            p.add_vol();
                        } else {
                            p.dec_vol();
                        }
                    }
                }
            } else if (event.type == sf::Event::MouseButtonPressed) {
                auto [id, idx] = songs.get_click_id(event.mouseButton.x, event.mouseButton.y);
                if (id != -1) {
                    songs.grab(event.mouseButton.y);
                    if (id != p.get_id()) {
                        p.play_id(id);
                    }
                } else if (display.bar.in_bar(event.mouseButton.x, event.mouseButton.y)) {
                    display.bar.holding = true;
                    p.pause();
                    display.bar.set_position(&p, event.mouseButton.x);
                }
            } else if (event.type == sf::Event::MouseMoved) {
                if (display.bar.holding) {
                    display.bar.set_position(&p, event.mouseMove.x);
                }
            } else if (event.type == sf::Event::MouseButtonReleased) {
                if (songs.holding) {
                    songs.release(event.mouseButton.y, clock.getElapsedTime());
                }
                if (display.bar.holding) {
                    display.bar.set_position(&p, event.mouseButton.x);
                    display.bar.holding = false;
                    p.play();
                }
            } else if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Space) {
                    if (songSearch.empty()) {
                        if (p.is_playing()) {
                            p.pause();
                        } else {
                            p.play();
                        }
                    }
                } else if (event.key.code == sf::Keyboard::V && event.key.control) {
                    visualiser.display ^= 1;
                } else if (event.key.code == sf::Keyboard::C && event.key.control) {
                    sf::Clipboard::setString(p.getSong().artist + L" — " + p.getSong().title);
                } else if (event.key.code == sf::Keyboard::Left) {
                    p.backward_5();
                } else if (event.key.code == sf::Keyboard::Right) {
                    p.forward_5();
                } else if (event.key.code == sf::Keyboard::Up) {
                    if (sortSelect.state) {
                        sortSelect.up();
                    } else {
                        songs.play_prev();
                    }
                    songs.norm_shift_tile();
                } else if (event.key.code == sf::Keyboard::Down) {
                    if (sortSelect.state) {
                        sortSelect.down();
                    } else {
                        songs.play_next();
                    }
                    songs.norm_shift_tile();
                } else if (event.key.code == sf::Keyboard::Enter) {
                    if (sortSelect.state) {
                        sortSelect.sort(&p);
                        songs.init(&p);
                        sortSelect.state = false;
                    } else if (!songSearch.empty()) {
                        p.play_id(p.get_first_id(songSearch.get_filter()));
                    }
                    songs.norm_shift_tile();
                } else if (event.key.code == sf::Keyboard::R && event.key.control) {
                    p.loop ^= 1;
                } else if (event.key.code == sf::Keyboard::Escape) {
                    if (sortSelect.state) {
                        sortSelect.state = false;
                    } else if (!songSearch.empty()) {
                        songSearch.clear();
                        songs.init(&p, songSearch.get_filter());
                        songSearch.update_color(songs.size());
                    }
                } else if (event.key.code == sf::Keyboard::BackSpace) {
                    if (!songSearch.empty()) {
                        if (event.key.control) {
                            songSearch.pop_word();
                            songs.init(&p, songSearch.get_filter());
                            songSearch.update_color(songs.size());
                        } else {
                            songSearch.pop_char();
                            songs.init(&p, songSearch.get_filter());
                        }
                        songSearch.update_color(songs.size());
                    }
                } else if (event.key.code == sf::Keyboard::PageDown) {
                    songs.pagedown();
                } else if (event.key.code == sf::Keyboard::PageUp) {
                    songs.pageup();
                } else if (event.key.code == sf::Keyboard::F5) {
                    settings.load();
                } else if (event.key.code == sf::Keyboard::F6) {
                    if (songSearch.empty()) {
                        sortSelect.state ^= 1;
                    }
                } else if (event.key.code == sf::Keyboard::I && event.key.control) {
                    Tile::LOAD_IMG ^= 1;
                }  else if (event.key.code == -1) {
                    p.is_playing() ? p.pause(): p.play();
                } else if (event.key.code == sf::Keyboard::Q && event.key.control) {
                    window.close();
                    return 0;
                }
            } else if (event.type == sf::Event::TextEntered && !sortSelect.state) {
                int u = event.text.unicode;
                if (u != 27 && u != 13 && u != 8 && !sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl)) {
//                    std::wcout << L"ASCII character typed: " << static_cast<wchar_t>(event.text.unicode) << ' '
//                              << event.text.unicode << std::endl;
                    if (event.text.unicode != 32 || !songSearch.empty()) {
                        songSearch.add_char(event.text.unicode);
                        songs.init(&p, songSearch.get_filter());
                        songSearch.update_color(songs.size());
                    }
                }
            }
        }
        if (visualiser.display && p.is_playing()) {
            p.get_fft(fft);
        }
        if (songs.holding) {
            songs.set_position(sf::Mouse::getPosition(window).y);
        }
        stars_rot(stars_vec);
        starfield.move(stars_vec);
        window.clear();
        window.draw(starfield);
        visualiser.render(window, fft);
        songs.render(window, font, bold_font, clock.getElapsedTime());
        songSearch.render(window, font);
        display.render(window, font, bold_font);
        vol_slider.render(window, bold_font, clock.getElapsedTime());
        sortSelect.render(window, font);
        window.display();
    }

    return 0;
}

// TODO: folders support
// TODO: scroll faster with shift, don't load images if scrolling with shift
// TODO: song duration in song list
// TODO: GUI for downloading songs from Internet (yt-dlp?)
// TODO: shaders
// TODO: somehow compile for windows (in distant future)
// TODO: write help menu, write readme
// TODO: add different file formats support (flac?)

