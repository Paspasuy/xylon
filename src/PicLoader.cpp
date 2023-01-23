#include "PicLoader.h"

#include <thread>

PicLoader::PicLoader(sf::Clock* cl) : cl(cl) {}

void PicLoader::load(Song* pSong) {
    pSong->pic_loading = true;
    mutex.lock();
    to_load.emplace(pSong);
    mutex.unlock();
}

void PicLoader::update() {
    mutex.lock();
    if (!to_load.empty()) {
        while (to_load.size() > MAX_QUEUE) {
            Song* cur = to_load.front();
            to_load.pop();
            cur->pic_loading = false;
        }
        Song* cur = to_load.front();
        to_load.pop();
        mutex.unlock();
        std::thread thr(load_ptr, cur, cl);
        thr.detach();
    } else {
        mutex.unlock();
    }
}

void PicLoader::load_ptr(Song* cur, sf::Clock* cl) { cur->load_pic(cl); }
