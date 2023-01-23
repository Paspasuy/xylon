#include "MainWindow.h"

void stars_rot(sf::Vector2f& vec, float sin = 0.001) {
    float x1 = vec.x;
    float y1 = vec.y;
    float cos = 1 - sin * sin;
    vec.x = cos * x1 - sin * y1;
    vec.y = sin * x1 + cos * y1;
}

MainWindow::MainWindow(sf::ContextSettings ctxsettings)
    : sf::RenderWindow(sf::VideoMode(1024, 768), "xylon", sf::Style::Default, ctxsettings),
      starfield(sf::Vector2f(1024, 768), 700),
      p(&clock),
      songDisplay(&p, &settings),
      vol_slider(&p, clock.getElapsedTime()),
      songs(&settings),
      pl(&clock),
      visualiser(&settings),
      stars_vec(0.4, -0.1) {
    setVerticalSyncEnabled(true);

    for (std::string& s : settings.folders) {
        p.add_folder(s);
    }
    p.sort_by_date();
    p.ptr = 0;

    Tile::W = std::min(int(getSize().x) / 2, Tile::MAX_W);

    if (!font.loadFromFile("/usr/share/fonts/adobe-source-code-pro/SourceCodePro-Regular.otf")) {
        std::cerr << "FONTS BROKEN\n";
    }

    if (!bold_font.loadFromFile("/usr/share/fonts/adobe-source-code-pro/SourceCodePro-Bold.otf")) {
        std::cerr << "FONTS BROKEN\n";
    }

    songs.init(&p);
    p.play();

    stars_rot(stars_vec, (rand() % 100) / 100.f);
}

void MainWindow::pollEvents() {
    sf::Event event;
    while (pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            close();
        }
        if (event.type == sf::Event::Resized) {
            Tile::W = std::min(int(getSize().x) / 2, Tile::MAX_W);
            sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
            setView(sf::View(visibleArea));
            songs.winsz = getSize();
            starfield.regenerate(sf::Vector2f(getSize().x, getSize().y));
        } else if (event.type == sf::Event::MouseWheelScrolled) {
            if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
                int delta = event.mouseWheelScroll.delta;
                if (songs.get_click_id(event.mouseWheelScroll.x, event.mouseWheelScroll.y).first !=
                    -1) {
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
            } else if (songDisplay.bar.in_bar(event.mouseButton.x, event.mouseButton.y)) {
                songDisplay.bar.holding = true;
                p.pause();
                songDisplay.bar.set_position(&p, event.mouseButton.x);
            }
        } else if (event.type == sf::Event::MouseMoved) {
            if (songDisplay.bar.holding) {
                songDisplay.bar.set_position(&p, event.mouseMove.x);
            }
        } else if (event.type == sf::Event::MouseButtonReleased) {
            if (songs.holding) {
                songs.release(event.mouseButton.y, clock.getElapsedTime());
            }
            if (songDisplay.bar.holding) {
                songDisplay.bar.set_position(&p, event.mouseButton.x);
                songDisplay.bar.holding = false;
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
                pl.should_load ^= 1;
            } else if (event.key.code == -1) {
                p.is_playing() ? p.pause() : p.play();
            } else if (event.key.code == sf::Keyboard::Q && event.key.control) {
                close();
            }
        } else if (event.type == sf::Event::TextEntered && !sortSelect.state) {
            int u = event.text.unicode;
            if (u != 27 && u != 13 && u != 8 &&
                !sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl)) {
                //                    std::wcout << L"ASCII character typed: " <<
                //                    static_cast<wchar_t>(event.text.unicode) << ' '
                //                              << event.text.unicode << std::endl;
                if (event.text.unicode != 32 || !songSearch.empty()) {
                    songSearch.add_char(event.text.unicode);
                    songs.init(&p, songSearch.get_filter());
                    songSearch.update_color(songs.size());
                }
            }
        }
    }
}

void MainWindow::beforeRender(uint64_t frame) {
    if (visualiser.display && p.is_playing()) {
        p.get_fft(fft);
    }
    if (songs.holding) {
        songs.set_position(sf::Mouse::getPosition(*this).y);
    }
    songs.shift_pressed = sf::Keyboard::isKeyPressed(sf::Keyboard::LShift);
    if ((frame & 1) == 0) {
        pl.update();
    }

    stars_rot(stars_vec);
    starfield.move(stars_vec);
}

void MainWindow::render() {
    clear();
    draw(starfield);
    visualiser.render(*this, fft);
    songs.render(*this, pl, clock, font, bold_font, clock.getElapsedTime());
    songSearch.render(*this, font);
    songDisplay.render(*this, clock, font, bold_font);
    vol_slider.render(*this, bold_font, clock.getElapsedTime());
    sortSelect.render(*this, font);
    display();
}

void MainWindow::beforePolling() {
    if (clock.getElapsedTime() > p.expire && p.is_playing()) {
        songs.play_next();
    }
}