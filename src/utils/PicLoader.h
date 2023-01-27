#ifndef XYLON_PICLOADER_H
#define XYLON_PICLOADER_H

#include <mutex>
#include <queue>

#include "../Song.h"
#include "Utils.h"

class PicLoader {
    static std::mutex mutex;
    static std::queue<Song*> to_load;

    static void load_from_queue();
public:
    bool should_load = true;
    void load(Song* pSong);
    void update();
    const int MAX_QUEUE = 15;
};

#endif  // XYLON_PICLOADER_H
