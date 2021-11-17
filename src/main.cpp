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

void stars_rot(sf::Vector2f &vec, float sin=0.001) {
    float x1 = vec.x;
    float y1 = vec.y;
    float cos = 1 - sin * sin;
    vec.x = cos * x1 - sin * y1;
    vec.y = sin * x1 + cos * y1;
}

int main() {
    srand(time(0));
    int winw = 1024;
    int winh = 768;
    std::locale::global(std::locale("ru_RU.utf-8"));
    std::wcin.imbue(std::locale("ru_RU.utf-8"));
    std::wcout.imbue(std::locale("ru_RU.utf-8"));
    std::string path_to_music = std::string(getenv("HOME")) + "/Music/";
    BASS_Init(1, 44100, 0, 0, NULL);
    auto *clock = new sf::Clock();
    auto *p = new Player(clock);
    p->add_folder(path_to_music);
    for (auto &it: p->songs) {
        it->add_meta();
    }
    auto *cpl = p;
    auto *display = new SongDisplay(p);
    VolumeCircleSlider vol_slider(p, clock->getElapsedTime());
    std::sort(p->songs.begin(), p->songs.end(), [&](Song *i, Song *j) {
        return std::make_pair(i->album, i->artist) < std::make_pair(j->album, j->artist);
    });
//    p -> add_song("/home/pavel/Music/amogus2.wav");
    sw::Starfield starfield(sf::Vector2f(winw, winh), 700);
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode(winw, winh), "xylon", sf::Style::Default, settings);
    Tile::W = std::min(int(window.getSize().x) / 2, Tile::MAX_W);
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
    auto *songSearch = new SongSearch(p);
    sf::Vector2f stars_vec(0.4, -0.1);
    stars_rot(stars_vec, (rand() % 100) / 100.f);
    sf::Time prev = clock->getElapsedTime();
    while (window.isOpen()) {
        if (clock->getElapsedTime() > p->expire && p->is_playing()) {
            if (songSearch->state()) {
                ++cpl->ptr;
                cpl->ptr %= cpl->songs.size();
                p->play_id(cpl->get_current_id());
            } else {
                p->next();
            }
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
                        vol_slider.touch(clock->getElapsedTime());
                        if (delta == 1) {
                            p->add_vol();
                        } else {
                            p->dec_vol();
                        }
                    }
                }
            } else if (event.type == sf::Event::MouseButtonPressed) {
                auto [id, idx] = songs.get_click_id(event.mouseButton.x, event.mouseButton.y);
                if (id != -1) {
                    songs.grab(event.mouseButton.y);
                    if (songSearch->state()) {
                        cpl->set_index(idx);
                    }
                    if (id != p->get_current_id()) {
                        p->play_id(id);
                    }
                } else if (display->bar.in_bar(event.mouseButton.x, event.mouseButton.y)) {
                    display->bar.holding = 1;
                    p->pause();
                    display->bar.set_position(p, event.mouseButton.x);
                }
            } else if (event.type == sf::Event::MouseMoved) {
                if (display->bar.holding) {
                    display->bar.set_position(p, event.mouseMove.x);
                }
            } else if (event.type == sf::Event::MouseButtonReleased) {
                if (songs.holding) {
                    songs.release(event.mouseButton.y, clock->getElapsedTime());
                }
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
                    if (songSearch->state()) {
                        --cpl->ptr;
                        cpl->ptr += cpl->songs.size() * 2;
                        cpl->ptr %= cpl->songs.size();
                        p->play_id(cpl->get_current_id());
                    } else {
                        p->prev();
                        songs.norm_shift_up();
                    }
                } else if (event.key.code == sf::Keyboard::Down) {
                    if (songSearch->state()) {
                        ++cpl->ptr;
                        cpl->ptr %= cpl->songs.size();
                        p->play_id(cpl->get_current_id());
                    } else {
                        p->next();
                        songs.norm_shift_down();
                    }
                } else if (event.key.code == sf::Keyboard::Enter) {
                    if (songSearch->state()) {
                        int id = cpl->songs[0]->id;
                        p->play_id(id);
                        cpl->ptr = 0;
                    }
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
//                    std::wcout << L"ASCII character typed: " << static_cast<wchar_t>(event.text.unicode) << ' '
//                              << event.text.unicode << std::endl;
                    if (event.text.unicode != 32 || songSearch->state()) {
                        cpl = songSearch->add_char(event.text.unicode);
                        songs.init(cpl);
                    }
                }
            }
        }
        if (songs.holding) {
            songs.set_position(sf::Mouse::getPosition(window).y);
        }
        stars_rot(stars_vec);
        starfield.move(stars_vec);
        window.clear();
        window.draw(starfield);
        songs.render(window, font, bold_font, clock->getElapsedTime());
        songSearch->render(window, font);
        display->render(window, font, bold_font);
        vol_slider.render(window, bold_font, clock->getElapsedTime());
        sf::Time cur = clock->getElapsedTime();
        sf::Text fps(std::to_string(int(1.f / (cur - prev).asSeconds() + 2) / 10 * 10), font, 20);
        fps.setFillColor(sf::Color(255, 0, 0, 150));
        fps.setPosition(0.f, 0.f);
        window.draw(fps);
        window.display();
        prev = cur;
    }

    return 0;
}

// TODO: albums support
// TODO: maybe add second margin (Oy)
// TODO: add visualiser
// TODO: settings in config file
// TODO: add icon for repeating
// TODO: add ability to change song metadata
// TODO: resize album picture properly
