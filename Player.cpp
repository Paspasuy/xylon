#include <SFML/System/Clock.hpp>
#include <vector>
#include "Song.cpp"
#include <filesystem>

#ifndef PLAYER
#define PLAYER

class Player {
public:
    int ptr = 0;
    bool state;
    double vol = 1.;
    sf::Clock *c;
    double progress;
    bool loop = 0;

    sf::Time expire;

    Player(sf::Clock *_c) {
        c = _c;
    }

/*    Player(Player *pl) {
        c = pl->c;
        state = pl->state;
        vol = pl->vol;
        progress = pl->progress;
        loop = pl->loop;
    }*/

    Player(Player *pl) {
        c = pl->c;
        state = 0;
        vol = 1;
        progress = 0;
        ptr = 0;
        loop = 0;
    }

    std::vector<Song *> songs;

    void upd_expire() {
        expire = c->getElapsedTime() + songs[ptr]->getDuration() - songs[ptr]->getPlayingOffset();
    }

    void play() {
        state = 1;
        upd_expire();
        songs[ptr]->set_vol(vol);
        songs[ptr]->play();
    }

    void pause() {
        state = 0;
        songs[ptr]->pause();
    }

    void stop() {
        state = 0;
        songs[ptr]->stop();
    }

    void next() {
        stop();
        if (!loop) {
            ++ptr;
        }
        ptr %= songs.size();
        play();
    }

    void play_ind(int ind) {
        stop();
        ptr = ind;
        play();
    }

    void play_id(int id) {
        for (int i = 0; i < songs.size(); ++i) {
            if (songs[i]->id == id) {
                play_ind(i);
                return;
            }
        }
    }

    void prev() {
        stop();
        --ptr;
        if (ptr < 0) {
            ptr = songs.size() - 1;
        }
        play();
    }

    void add_song(std::string s) {
        songs.push_back(new Song(s));
    }

    bool is_playing() {
        return state;
    }

    void add_folder(std::string s) {
        using iter = std::filesystem::recursive_directory_iterator;
        for (const auto &dirEntry: iter(s)) {
            if (dirEntry.path().extension() == ".mp3") {
                add_song(dirEntry.path());
            }
        }

    }

    void set_vol(double x) {
        vol = x;
        if (state) {
            songs[ptr]->set_vol(x);
        }
    }

    int current_index() {
        return ptr;
    }

    void add_vol() {
        set_vol(std::min(1., vol + 0.05));
    }

    void dec_vol() {
        set_vol(std::max(0., vol - 0.05));
    }

    void backward_5() {
        songs[ptr]->backward_5();
        upd_expire();
    }

    void forward_5() {
        songs[ptr]->forward_5();
        upd_expire();
    }

    sf::Time getDuration() {
        return songs[ptr]->getDuration();
    }

    sf::Time getPlayingOffset() {
        return songs[ptr]->getPlayingOffset();
    }

    void set_position(double d) {
        double t = songs[ptr]->getDuration().asSeconds() * d;
        songs[ptr]->set_position(t);
    }

};

#endif