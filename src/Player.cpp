#include "Player.h"

#include <random>

void Player::upd_expire() {
    expire = clk.getElapsedTime() + songs[ptr].getDuration() - songs[ptr].getPlayingOffset();
}

void Player::play() {
    playing = true;
    upd_expire();
    songs[ptr].set_vol(vol);
    songs[ptr].play();
}

void Player::pause() {
    playing = false;
    songs[ptr].pause();
}

void Player::stop() {
    playing = false;
    if (!songs.empty()) songs[ptr].stop();
}

void Player::next(bool ignore_loop) {
    stop();
    if (ignore_loop || !loop) {
        ++ptr;
    }
    if (ptr >= songs.size()) ptr = 0;
    play();
}

void Player::play_ind(int ind) {
    stop();
    ptr = ind;
    play();
}

void Player::play_id(int id) { play_ind(index_by_id(id)); }

void Player::prev() {
    stop();
    ptr ? --ptr : ptr = songs.size() - 1;
    play();
}

void Player::add_song(const std::string& s, const std::u8string& t, time_t time) {
    songs.emplace_back(Song(s, t, time));
    songs.back().add_meta();
}

bool Player::is_playing() { return playing; }

void Player::add_folder(const std::string& path) {
    using iter = std::filesystem::recursive_directory_iterator;
    for (const auto& dirEntry : iter(path)) {
        if (dirEntry.path().extension() == ".mp3") {
            time_t time = std::chrono::system_clock::to_time_t(
                std::chrono::file_clock::to_sys(dirEntry.last_write_time()));
            add_song(dirEntry.path().string(), dirEntry.path().filename().u8string(), time);
        }
    }
}

void Player::set_vol(double x) {
    vol = x;
    if (playing) {
        songs[ptr].set_vol(x);
    }
}

void Player::add_vol() { set_vol(std::min(1., vol + 0.05)); }

void Player::dec_vol() { set_vol(std::max(0., vol - 0.05)); }

void Player::backward_5() {
    songs[ptr].backward_5();
    upd_expire();
}

void Player::forward_5() {
    songs[ptr].forward_5();
    upd_expire();
}

sf::Time Player::getDuration() { return songs[ptr].getDuration(); }

sf::Time Player::getPlayingOffset() { return songs[ptr].getPlayingOffset(); }

void Player::set_position(double d) {
    double t = songs[ptr].getDuration().asSeconds() * d;
    songs[ptr].set_position(t);
}

Song& Player::getSong() { return songs[ptr]; }

int Player::get_id() { return songs[ptr].id; }

void Player::get_fft(float* fft) { songs[ptr].get_fft(fft); }

void Player::sort_by_album() {
    int id = songs[ptr].id;
    std::sort(songs.begin(), songs.end(), [](Song& i, Song& j) {
        return std::make_pair(i.album, i.track) < std::make_pair(j.album, j.track);
    });
    ptr = index_by_id(id);
}

void Player::sort_by_date() {
    int id = songs[ptr].id;
    std::sort(songs.begin(), songs.end(), [](Song& i, Song& j) { return i.cr_time < j.cr_time; });
    std::reverse(songs.begin(), songs.end());
    ptr = index_by_id(id);
}

void Player::sort_by_random() {
    int id = songs[ptr].id;
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    shuffle(songs.begin(), songs.end(), std::default_random_engine(seed));
    ptr = index_by_id(id);
}

int Player::index_by_id(int id) {
    for (int i = 0; i < songs.size(); ++i) {
        if (songs[i].id == id) {
            return i;
        }
    }
    return 0;
}

std::vector<Song*> Player::get_songs(const std::wstring& filter) {
    std::vector<Song*> res;
    for (auto& song : songs) {
        if (song.matches(filter)) {
            res.emplace_back(&song);
        }
    }
    return res;
}

int Player::get_first_id(const std::wstring& filter) {
    for (auto& song : songs) {
        if (song.matches(filter)) {
            return song.id;
        }
    }
    return -1;
}

uint64_t Player::current_id() { return songs[ptr].id; }

void Player::reset() {
    stop();
    songs.clear();
    playing = false;
    ptr = 0;
    loop = false;
}
