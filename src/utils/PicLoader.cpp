#include "PicLoader.h"

#include <thread>

void PicLoader::load(Song* pSong) {
    threadPool.queueJob(PicLoader::load_from_queue, 0);
    pSong->pic_loading = true;
    std::unique_lock<std::mutex> lock(mutex);
    to_load.emplace(pSong);
}

void PicLoader::update() {
    std::unique_lock<std::mutex> lock(mutex);
    while (to_load.size() > MAX_QUEUE) {
        Song* cur = to_load.front();
        to_load.pop();
        cur->pic_loading = false;
    }
}

void PicLoader::load_from_queue() {
    Song* cur;
    {
        std::unique_lock<std::mutex> lock(mutex);
        if (to_load.empty()) return;
        cur = to_load.front();
        to_load.pop();
    }
    cur->load_pic();
}


std::mutex PicLoader::mutex;
std::queue<Song*> PicLoader::to_load;