#ifndef XYLON_PICLOADER_H
#define XYLON_PICLOADER_H

#include <mutex>
#include <queue>
#include "Song.h"

class PicLoader {
    std::mutex mutex;
    std::queue<Song*> to_load;
    sf::Clock* cl;
    static void load_ptr(Song* cur, sf::Clock* cl);

public:
    explicit PicLoader(sf::Clock* cl);
    bool should_load = true;
    void load(Song* pSong);
    void update();
    const int MAX_QUEUE = 15;
};

#endif  // XYLON_PICLOADER_H
