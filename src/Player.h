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
    double vol = 1.;
    sf::Clock *c;
    double progress;
    bool loop = 0;

    sf::Time expire;

    Player(sf::Clock *_c);

    Player(Player *pl);

    std::vector<Song *> songs;

    void upd_expire();

    void play();

    void pause();

    void stop();

    void next();

    void play_ind(int ind);

    void play_id(int id);

    void prev();

    void add_song(std::string s);

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

    Song *getSong();

    void set_index(int idx);

    int get_current_id();
};

#endif //XYLON_PLAYER_H
