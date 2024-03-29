#ifndef XYLON_PLAYER_H
#define XYLON_PLAYER_H

#include <SFML/System/Clock.hpp>
#include <filesystem>
#include <vector>

#include "Song.h"
#include "utils/Utils.h"

class Player {
    bool playing;
    std::vector<Song> songs;

    int index_by_id(uint64_t id);

public:
    double vol = 0.5;
    int ptr = 0;
    bool loop = false;

    sf::Time expire;

    void upd_expire();

    void play();

    void pause();

    void stop();

    void next(bool ignore_loop = false);

    void play_ind(int ind);

    void play_id(uint64_t id);

    void prev();

    void add_song(const std::string& s, const std::string& t, time_t time);

    bool is_playing();

    void add_folder(const std::string& path);

    void set_vol(double x);

    void add_vol();

    void dec_vol();

    void backward_5();

    void forward_5();

    sf::Time getDuration();

    sf::Time getPlayingOffset();

    void set_position(double d);

    Song& getSong();

    int get_id();

    void get_fft(float* fft);

    void sort_by_album();

    void sort_by_date();

    void sort_by_random();

    std::vector<Song*> get_songs(const std::wstring& filter, const std::wstring& album);

    std::vector<Song*> filter_by_album(const std::wstring& album);

    std::vector<std::wstring> get_albums();

    int get_first_id(const std::wstring& filter, const std::wstring& album);

    size_t current_id();

    void reset();

    bool empty();
};

#endif  // XYLON_PLAYER_H
