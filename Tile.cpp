#ifndef TILE
#define TILE

#include "Song.cpp"
#include <taglib/mpegfile.h>
#include <taglib/fileref.h>

class Tile {
public:
    static const int W = 500;
    static const int H = 80;
    sf::String title = "Title";
    sf::String artist = "John Doe";
    sf::String album = "Default Album";
    sf::Vector2i position;

    Tile(Song *s) {
        TagLib::FileRef f((s->path).c_str());
        title = f.tag()->title().toWString();
        if (title.getSize() == 0) {
            title = s->path;
        }
        artist = f.tag()->artist().toWString();
        album = f.tag()->artist().toWString();
//        TagLib::String artist = f.tag()->artist();
//        TagLib::FileRef g("Free City Rhymes.ogg");
//        TagLib::String album = g.tag()->album(); // album == "NYC Ghosts & Flowers"
//
//        g.tag()->setTrack(1);
    }
};

#endif
