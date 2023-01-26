#include "Song.h"

#include <cmath>
#include <codecvt>

Song::Song(const std::string& path, const std::string& filename, time_t cr_time)
    : filename(filename), cr_time(cr_time), path(path) {
    id = rand();
    channel = BASS_StreamCreateFile(FALSE, path.c_str(), 0, 0, BASS_SAMPLE_FLOAT);
    int err = BASS_ErrorGetCode();
    if (err != 0) {
        std::cerr << err << std::endl;
        std::cerr << "CHANNEL: " << channel << std::endl;
        std::cerr << "SONG: " << path << std::endl;
    }
}

void Song::play() { BASS_ChannelPlay(channel, FALSE); }

void Song::pause() { BASS_ChannelPause(channel); }

void Song::stop() {
    BASS_ChannelPause(channel);
    BASS_ChannelSetPosition(channel, 0, BASS_POS_BYTE);
}

void Song::set_vol(double x) { BASS_ChannelSetAttribute(channel, BASS_ATTRIB_VOL, x); }

sf::Time Song::getDuration() {
    QWORD len = BASS_ChannelGetLength(channel, BASS_POS_BYTE);  // the length in bytes
    double time = BASS_ChannelBytes2Seconds(channel, len);      // the length in seconds
    return sf::microseconds(int64_t(time * 1000 * 1000));
}

sf::Time Song::getPlayingOffset() {
    QWORD len = BASS_ChannelGetPosition(channel, BASS_POS_BYTE);  // the length in bytes
    double time = BASS_ChannelBytes2Seconds(channel, len);        // the length in seconds
    return sf::microseconds(int64_t(time * 1000 * 1000));
}

void Song::backward_5() {
    QWORD len = BASS_ChannelGetPosition(channel, BASS_POS_BYTE);  // the length in bytes
    double time = BASS_ChannelBytes2Seconds(channel, len);        // the length in seconds
    time = std::max(0., time - 5);
    BASS_ChannelSetPosition(channel, BASS_ChannelSeconds2Bytes(channel, time), BASS_POS_BYTE);
}

void Song::forward_5() {
    QWORD len = BASS_ChannelGetPosition(channel, BASS_POS_BYTE);  // the length in bytes
    double time = BASS_ChannelBytes2Seconds(channel, len);        // the length in seconds
    time = std::min(double(getDuration().asSeconds()), time + 5);
    BASS_ChannelSetPosition(channel, BASS_ChannelSeconds2Bytes(channel, time), BASS_POS_BYTE);
}

void Song::set_position(double pos) {
    BASS_ChannelSetPosition(channel, BASS_ChannelSeconds2Bytes(channel, pos), BASS_POS_BYTE);
}

bool Song::matches(std::wstring text) {
    if (ltitle.find(text) != std::string::npos) return true;
    if (lartist.find(text) != std::string::npos) return true;
    if (lalbum.find(text) != std::string::npos) return true;
    return false;
}

void Song::add_meta() {
    TagLib::FileRef f(path.c_str());
    title = f.tag()->title().toWString();
    artist = f.tag()->artist().toWString();
    album = f.tag()->album().toWString();
    track = f.tag()->track();
    if (title.size() == 0) {
        title = std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(filename);
    }
    ltitle = lower(title);
    lartist = lower(artist);
    lalbum = lower(album);
}

void Song::load_pic() {
    TagLib::MPEG::File ff(path.c_str());
    pic_loaded = true;
    time_loaded = clk.getElapsedTime().asMilliseconds();
    if (ff.hasID3v2Tag()) {
        TagLib::ID3v2::Tag* id3V2Tag = ff.ID3v2Tag();
        auto l = id3V2Tag->frameList("APIC");
        if (l.size() == 0) {
            return;
        }
        auto* pic = dynamic_cast<TagLib::ID3v2::AttachedPictureFrame*>(l.front());
        //        int sz = static_cast<int>(std::sqrt(pic->picture().size() / 3));
        texture = new sf::Texture();
        texture->loadFromMemory((const u_char*)pic->picture().data(), pic->picture().size(),
                                sf::IntRect(0, 0, 1200, 1200));
        texture->setSmooth(true);
        sprite = new sf::Sprite(*texture);
        small_sprite = new sf::Sprite(*texture);
        int x = texture->getSize().x;
        float scale = 300.f / x;
        sprite->setScale({scale, scale});
        scale *= 78.f / 300.f;
        small_sprite->setScale({scale, scale});
    }
}

void Song::get_fft(float* fft) { BASS_ChannelGetData(channel, fft, BASS_DATA_FFT4096); }

Song::~Song() {
    delete sprite;
    delete small_sprite;
    delete texture;
    if (channel != 0) {
        BASS_ChannelFree(channel);
    }
}

Song::Song(Song&& other) noexcept
    : channel(other.channel),
      title(other.title),
      artist(other.artist),
      album(other.album),
      track(other.track),
      ltitle(other.ltitle),
      lartist(other.lartist),
      lalbum(other.lalbum),
      path(other.path),
      cr_time(other.cr_time),
      filename(other.filename),
      texture(other.texture),

      sprite(other.sprite),
      small_sprite(other.small_sprite),
      time_loaded(other.time_loaded),
      pic_loaded(other.pic_loaded),
      pic_loading(other.pic_loading),
      id(other.id) {
    other.sprite = nullptr;
    other.small_sprite = nullptr;
    other.texture = nullptr;
    other.channel = 0;
}

Song& Song::operator=(Song&& other) noexcept {
    channel = other.channel;
    title = other.title;
    artist = other.artist;
    album = other.album;
    track = other.track;
    ltitle = other.ltitle;
    lartist = other.lartist;
    lalbum = other.lalbum;
    path = other.path;
    cr_time = other.cr_time;
    filename = other.filename;
    texture = other.texture;

    sprite = other.sprite;
    small_sprite = other.small_sprite;
    time_loaded = other.time_loaded;
    pic_loaded = other.pic_loaded;
    pic_loading = other.pic_loading;
    id = other.id;

    other.sprite = nullptr;
    other.small_sprite = nullptr;
    other.texture = nullptr;
    other.channel = 0;

    return *this;
}
