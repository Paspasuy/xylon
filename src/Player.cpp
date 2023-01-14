#include <random>
#include "Player.h"

Player::Player(sf::Clock *_c) {
    c = _c;
}

Player::Player(Player *pl) {
    c = pl->c;
    state = false;
    vol = 1;
    progress = 0;
    ptr = 0;
    loop = false;
}

void Player::upd_expire() {
    expire = c->getElapsedTime() + songs[ptr].getDuration() - songs[ptr].getPlayingOffset();
}

void Player::play() {
    state = true;
    upd_expire();
    songs[ptr].set_vol(vol);
    songs[ptr].play();
}

void Player::pause() {
    state = false;
    songs[ptr].pause();
}

void Player::stop() {
    state = false;
    songs[ptr].stop();
}

void Player::next(bool ignore_loop) {
    stop();
    if (ignore_loop || !loop) {
        ++ptr;
    }
    ptr %= songs.size();
    play();
}

void Player::play_ind(int ind) {
    stop();
    ptr = ind;
    play();
}

void Player::play_id(int id) {
    for (int i = 0; i < songs.size(); ++i) {
        if (songs[i].id == id) {
            play_ind(i);
            return;
        }
    }
}

void Player::prev() {
    stop();
    --ptr;
    if (ptr < 0) {
        ptr = songs.size() - 1;
    }
    play();
}

void Player::add_song(const std::string &s, const std::u8string &t, time_t time) {
    songs.emplace_back(Song(s, t, time));
}

bool Player::is_playing() {
    return state;
}

void Player::add_folder(std::string s) {
    using iter = std::filesystem::recursive_directory_iterator;
    for (const auto &dirEntry: iter(s)) {
        if (dirEntry.path().extension() == ".mp3") {
            time_t time = std::chrono::system_clock::to_time_t(std::chrono::file_clock::to_sys(dirEntry.last_write_time()));
            add_song(dirEntry.path().string(), dirEntry.path().filename().u8string(), time);
        }
    }

}

void Player::set_vol(double x) {
    vol = x;
    if (state) {
        songs[ptr].set_vol(x);
    }
}

int Player::current_index() {
    return ptr;
}

void Player::add_vol() {
    set_vol(std::min(1., vol + 0.05));
}

void Player::dec_vol() {
    set_vol(std::max(0., vol - 0.05));
}

void Player::backward_5() {
    songs[ptr].backward_5();
    upd_expire();
}

void Player::forward_5() {
    songs[ptr].forward_5();
    upd_expire();
}

sf::Time Player::getDuration() {
    return songs[ptr].getDuration();
}

sf::Time Player::getPlayingOffset() {
    return songs[ptr].getPlayingOffset();
}

void Player::set_position(double d) {
    double t = songs[ptr].getDuration().asSeconds() * d;
    songs[ptr].set_position(t);
}

Song& Player::getSong() {
    return songs[ptr];
}

void Player::set_index(int idx) {
    ptr = idx;
}

int Player::get_id() {
    return songs[ptr].id;
}

void Player::get_fft(float* fft) {
    songs[ptr].get_fft(fft);
}

void Player::sort_by_album() {
    int id = songs[ptr].id;
    std::sort(songs.begin(), songs.end(), [](Song &i, Song &j) {
        return std::make_pair(i.album, i.track) < std::make_pair(j.album, j.track);
    });
    for (int i = 0; i < songs.size(); ++i) {
        if (songs[i].id == id) {
            ptr = i;
            return;
        }
    }
}

void Player::sort_by_date() {
    int id = songs[ptr].id;
    std::sort(songs.begin(), songs.end(), [](Song &i, Song &j) {
        return i.cr_time < j.cr_time;
    });
    std::reverse(songs.begin(), songs.end());
    for (int i = 0; i < songs.size(); ++i) {
        if (songs[i].id == id) {
            ptr = i;
            return;
        }
    }
}

void Player::sort_by_random() {
    int id = songs[ptr].id;
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    shuffle(songs.begin(), songs.end(), std::default_random_engine(seed));
    for (int i = 0; i < songs.size(); ++i) {
        if (songs[i].id == id) {
            ptr = i;
            return;
        }
    }
}

