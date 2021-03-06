//
// Created by pavel on 11/15/21.
//

#ifndef XYLON_SONG_H
#define XYLON_SONG_H

#include <SFML/System/Time.hpp>
#include <SFML/System/String.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include "bass.h"

#include <taglib/fileref.h>
#include <taglib/mpegfile.h>
#include <taglib/attachedpictureframe.h>
#include <taglib/id3v2tag.h>
#include <taglib/tag.h>

class Song {
    HCHANNEL channel;
public:
    sf::String title = "Title";
    sf::String artist = "John Doe";
    sf::String album = "Default Album";
    uint track = 0;
    std::wstring ltitle = L"title";
    std::wstring lartist = L"john doe";
    std::wstring lalbum = L"default album";
    std::string path;
    std::u8string filename;
    time_t cr_time;

    sf::Sprite sprite, small_sprite;
    sf::Texture texture;
    bool pic_loaded = 0;

    uint64_t id;

    Song(const std::string &_path, const std::u8string &_filename, time_t _cr_time);

    void play();

    void pause();

    void stop();

    void set_vol(double x);

    sf::Time getDuration();

    sf::Time getPlayingOffset();

    void backward_5();

    void forward_5();

    void set_position(double pos);

    bool matches(std::wstring text);

    void add_meta();

    void load_pic();

    static std::wstring lower(const std::wstring &s);

    void get_fft(float* fft);
};

#endif //XYLON_SONG_H
