//
// Created by pavel on 11/15/21.
//

#ifndef XYLON_SONG_H
#define XYLON_SONG_H

#include <taglib/attachedpictureframe.h>
#include <taglib/fileref.h>
#include <taglib/id3v2tag.h>
#include <taglib/mpegfile.h>
#include <taglib/tag.h>

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/String.hpp>
#include <SFML/System/Time.hpp>

#include "bass.h"
#include "utils/Utils.h"

class Song {
    HCHANNEL channel;

public:
    std::wstring title = L"Title";
    std::wstring artist = L"John Doe";
    std::wstring album = L"Default Album";
    uint track = 0;
    std::wstring ltitle = L"title";
    std::wstring lartist = L"john doe";
    std::wstring lalbum = L"default album";

    std::string path;
    std::string filename;
    time_t cr_time;

    sf::Sprite* sprite = nullptr;
    sf::Sprite* small_sprite = nullptr;
    sf::Texture* texture = nullptr;
    int time_loaded;
    bool pic_loaded = false;
    bool pic_loading = false;

    uint64_t id;

    Song() = delete;

    Song(const std::string& _path, const std::string& _filename, time_t _cr_time);

    Song& operator=(const Song&) = delete;
    Song(const Song&) = delete;

    Song& operator=(Song&& other) noexcept;
    Song(Song&& other) noexcept;

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

    void get_fft(float* fft);

    ~Song();
};

#endif  // XYLON_SONG_H
