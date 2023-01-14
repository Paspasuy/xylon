#ifndef XYLON_PLAYER_H
#define XYLON_PLAYER_H

#include <SFML/System/Clock.hpp>
#include <vector>
#include "Song.h"

#include <filesystem>

class Player {
public:
    int ptr = 0;
    bool state;
    double vol = 0.5;
    sf::Clock *c;
    double progress;
    bool loop = 0;

    sf::Time expire;

    Player(sf::Clock *_c);

    Player(Player *pl);

    std::vector<Song> songs;

    void upd_expire();

    void play();

    void pause();

    void stop();

    void next(bool ignore_loop = 0);

    void play_ind(int ind);

    void play_id(int id);

    void prev();

    void add_song(const std::string &s, const std::u8string &t, time_t time);

    bool is_playing();

    void add_folder(std::string s);

    void set_vol(double x);

    int current_index();

    void add_vol();

    void dec_vol();

    void backward_5();

    void forward_5();

    sf::Time getDuration();

    sf::Time getPlayingOffset();

    void set_position(double d);

    Song& getSong();

    void set_index(int idx);

    int get_id();

    void get_fft(float *fft);

    void sort_by_album();

    void sort_by_date();

    void sort_by_random();
};

#endif //XYLON_PLAYER_H
