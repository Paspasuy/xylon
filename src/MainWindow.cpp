#include "MainWindow.h"

void stars_rot(sf::Vector2f& vec, float sin = 0.001) {
    float x1 = vec.x;
    float y1 = vec.y;
    float cos = 1 - sin * sin;
    vec.x = cos * x1 - sin * y1;
    vec.y = sin * x1 + cos * y1;
}

MainWindow::MainWindow(sf::ContextSettings contextSettings)
    : sf::RenderWindow(sf::VideoMode(1024, 768), "xylon", sf::Style::Default, contextSettings),
      starfield(sf::Vector2f(1024, 768), 700),
      p(dirSelect.player),
      songDisplay(&p),
      vol_slider(&p),
      songList(picLoader),
      stars_vec(0.4, -0.1),
      postProcessing({1024, 768}, contextSettings) {
    setVerticalSyncEnabled(false);
    setFramerateLimit(settings.framerateLimit);

    for (std::string& s : settings.folders) {
        dirSelect.root.addChild(s);
    }
    dirSelect.init();
    p.ptr = 0;

    Tile::W = std::min(int(getSize().x) / 2, Tile::MAX_W);

    songList.winsz = getSize();
    songList.init(&p);
    albumSelect.init(p.get_albums());
    songList.playFirst();

    stars_rot(stars_vec, (rand() % 100) / 100.f);

    sf::Cursor cursor;
    uint8_t cur[4]{};
    if (cursor.loadFromPixels(cur, sf::Vector2u(1, 1), sf::Vector2u(0, 0))) setMouseCursor(cursor);
}

bool MainWindow::processGeneralEvent(sf::Event& event) {
    if (event.type == sf::Event::Closed) {
        close();
    } else if (event.type == sf::Event::Resized) {
        Tile::W = std::min(int(getSize().x) / 2, Tile::MAX_W);
        sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
        setView(sf::View(visibleArea));
        songList.winsz = getSize();
        starfield.regenerate(sf::Vector2f(getSize().x, getSize().y));
        postProcessing.create(sf::Vector2f(getSize().x, getSize().y));
    } else {
        return false;
    }
    return true;
}

bool MainWindow::processMouseEvent(sf::Event& event) {
    if (event.type == sf::Event::MouseWheelScrolled) {
        if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
            int delta = event.mouseWheelScroll.delta;
            if (songList.get_click_id(event.mouseWheelScroll.x, event.mouseWheelScroll.y).first !=
                -1) {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) {
                    delta *= 5;
                }
                songList.scroll(delta);
            } else {
                vol_slider.touch(clk.getElapsedTime());
                if (delta == 1) {
                    p.add_vol();
                } else {
                    p.dec_vol();
                }
            }
        }
    } else if (event.type == sf::Event::MouseButtonPressed) {
        mouseTrace.setClicked(true);
        auto [id, idx] = songList.get_click_id(event.mouseButton.x, event.mouseButton.y);
        if (id != -1) {
            songList.grab(event.mouseButton.y);
            if (id != p.get_id()) {
                p.play_id(id);
            }
        } else if (songDisplay.bar.in_bar(event.mouseButton.x, event.mouseButton.y)) {
            songDisplay.bar.holding = true;
            p.pause();
            songDisplay.bar.set_progress(&p, event.mouseButton.x);
        }
    } else if (event.type == sf::Event::MouseMoved) {
        mouseTrace.mousePosition = {event.mouseMove.x, event.mouseMove.y};
        if (songDisplay.bar.holding) {
            songDisplay.bar.set_progress(&p, event.mouseMove.x);
        }
    } else if (event.type == sf::Event::MouseButtonReleased) {
        mouseTrace.setClicked(false);
        if (songList.holding) {
            songList.release(event.mouseButton.y, clk.getElapsedTime());
        }
        if (songDisplay.bar.holding) {
            songDisplay.bar.set_progress(&p, event.mouseButton.x);
            songDisplay.bar.holding = false;
            p.play();
        }
    } else if (event.type == sf::Event::MouseLeft) {
        mouseTrace.clear();
    } else {
        return false;
    }
    return true;
}

void MainWindow::pollEvents() {
    sf::Event event;
    while (pollEvent(event)) {
        if (processGeneralEvent(event)) continue;
        if (processMouseEvent(event)) continue;
        if (event.type == sf::Event::KeyPressed) {
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
                if (sortSelect.show) {
                    sortSelect.up();
                } else if (dirSelect.show) {
                    dirSelect.up();
                } else if (albumSelect.show) {
                    albumSelect.up();
                } else {
                    songList.play_prev(true);
                    songList.norm_shift_tile();
                }
            } else if (event.key.code == sf::Keyboard::Down) {
                if (sortSelect.show) {
                    sortSelect.down();
                } else if (dirSelect.show) {
                    dirSelect.down();
                } else if (albumSelect.show) {
                    albumSelect.down();
                } else {
                    songList.play_next(true);
                    songList.norm_shift_tile();
                }
            } else if (event.key.code == sf::Keyboard::Enter) {
                if (!songSearch.empty()) {
                    if (dirSelect.show) {
                        try {
                            dirSelect.loadToPlayer();
                        } catch (std::runtime_error& err) {
                            close();
                        }
                        songList.init(&p);
                        albumSelect.init(p.get_albums());
                        dirSelect.filter("");
                        dirSelect.show = false;
                    } else if (albumSelect.show) {
                        albumSelect.album = albumSelect.visibleItems[albumSelect.ptr];
                        songList.init(&p, L"", albumSelect.album);
                        albumSelect.filter("");
                        albumSelect.show = false;
                    }
                    songSearch.clear();
                    songList.playFirst();
                } else if (sortSelect.show) {
                    sortSelect.applySort(&p);
                    songList.init(&p, L"", albumSelect.album);
                    sortSelect.show = false;
                } else if (albumSelect.show) {
                    albumSelect.album = albumSelect.visibleItems[albumSelect.ptr];
                    songList.init(&p, L"", albumSelect.album);
                    albumSelect.show = false;
                    songList.playFirst();
                } else if (dirSelect.show) {
                    try {
                        dirSelect.loadToPlayer();
                    } catch (std::runtime_error& err) {
                        close();
                    }
                    songList.init(&p);
                    albumSelect.init(p.get_albums());
                    dirSelect.show = false;
                    songList.playFirst();
                }
                songList.find_cur();
                songList.norm_shift_tile();
            } else if (event.key.code == sf::Keyboard::R && event.key.control) {
                p.loop ^= 1;
            } else if (event.key.code == sf::Keyboard::Escape) {
                if (!songSearch.empty()) {
                    songSearch.clear();
                    if (!dirSelect.show) {
                        songList.init(&p, L"", albumSelect.album);
                    } else {
                        dirSelect.filter("");
                    }
                } else if (sortSelect.show) {
                    sortSelect.show = false;
                } else if (dirSelect.show) {
                    dirSelect.show = false;
                } else if (albumSelect.show) {
                    albumSelect.show = false;
                }
            } else if (event.key.code == sf::Keyboard::BackSpace) {
                if (!songSearch.empty()) {
                    event.key.control ? songSearch.pop_word() : songSearch.pop_char();
                    if (dirSelect.show) {
                        dirSelect.filter(songSearch.get_filter());
                        songSearch.update_color(dirSelect.size());
                    } else if (albumSelect.show) {
                        albumSelect.filter(songSearch.get_filter());
                        songSearch.update_color(albumSelect.size());
                    } else {
                        songList.init(&p, songSearch.get_filter(), albumSelect.album);
                        songSearch.update_color(songList.size());
                    }
                }
            } else if (event.key.code == sf::Keyboard::PageDown) {
                if (sortSelect.show) {
                    sortSelect.pageDown();
                } else if (dirSelect.show) {
                    dirSelect.pageDown();
                } else if (albumSelect.show) {
                    albumSelect.pageDown();
                } else {
                    songList.pagedown();
                }
            } else if (event.key.code == sf::Keyboard::PageUp) {
                if (sortSelect.show) {
                    sortSelect.pageUp();
                } else if (dirSelect.show) {
                    dirSelect.pageUp();
                } else if (albumSelect.show) {
                    albumSelect.pageUp();
                } else {
                    songList.pageup();
                }
            } else if (event.key.code == sf::Keyboard::F5) {
                settings.load();
            } else if (event.key.code == sf::Keyboard::I && event.key.control) {
                picLoader.should_load ^= 1;
            } else if (event.key.code == -1) {
                p.is_playing() ? p.pause() : p.play();
            } else if (event.key.code == sf::Keyboard::Q && event.key.control) {
                close();
            } else if (event.key.code == sf::Keyboard::S && event.key.control) {
                if (songSearch.empty() && !dirSelect.show && !albumSelect.show) {
                    sortSelect.show ^= 1;
                }
            } else if (event.key.code == sf::Keyboard::O && event.key.control) {
                if (songSearch.empty() && !sortSelect.show && !albumSelect.show) {
                    dirSelect.show ^= 1;
                }
            } else if (event.key.code == sf::Keyboard::A && event.key.control) {
                if (songSearch.empty() && !sortSelect.show && !dirSelect.show) {
                    albumSelect.show ^= 1;
                }
            } else if (event.key.code == sf::Keyboard::D && event.key.control) {
                DownloadView::download(sf::Clipboard::getString(), dirSelect.currentPath);
            }
        } else if (event.type == sf::Event::TextEntered && !sortSelect.show) {
            int u = event.text.unicode;
            if (u != 27 && u != 13 && u != 8 &&
                !sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) &&
                !sf::Keyboard::isKeyPressed(sf::Keyboard::RControl)) {
                //                    std::wcout << L"ASCII character typed: " <<
                //                    static_cast<wchar_t>(event.text.unicode) << ' '
                //                              << event.text.unicode << std::endl;
                if (event.text.unicode != 32 || !songSearch.empty()) {
                    songSearch.add_char(event.text.unicode);
                    if (dirSelect.show) {
                        dirSelect.filter(songSearch.get_filter());
                        songSearch.update_color(dirSelect.size());
                    } else if (albumSelect.show) {
                        albumSelect.filter(songSearch.get_filter());
                        songSearch.update_color(albumSelect.size());
                    } else {
                        songList.init(&p, songSearch.get_filter(), albumSelect.album);
                        songSearch.update_color(songList.size());
                    }
                }
            }
        }
    }
}

void MainWindow::beforeRender(uint64_t frame) {
    if (visualiser.display && p.is_playing()) {
        p.get_fft(visualiser.fft);
    }
    if (songList.holding) {
        songList.set_position(mouseTrace.mousePosition.y);
    }
    songList.shift_pressed = sf::Keyboard::isKeyPressed(sf::Keyboard::LShift);
    picLoader.update();

    stars_rot(stars_vec);
    starfield.move(stars_vec);

    mouseTrace.push(mouseTrace.mousePosition);
}

void MainWindow::render() {
    clear();
    draw(starfield);
    draw(visualiser);
    draw(songList);
    draw(songDisplay);
    draw(sortSelect);
    draw(dirSelect);
    draw(albumSelect);
    draw(songSearch);
    draw(download);

    postProcessing.clear();
    postProcessing.add(vol_slider);
    postProcessing.add(mouseTrace);
    draw(postProcessing);

    display();
}

void MainWindow::beforePolling() {
    if (clk.getElapsedTime() > p.expire && p.is_playing()) {
        songList.play_next();
    }
    if (DownloadView::need_update) {
        DownloadView::need_update = false;
        p.reset();
        p.add_folder(dirSelect.currentPath);
        p.sort_by_date();
        songList.init(&p);
        p.play();
    }
}
