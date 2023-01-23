#include "Song.h"

#include <cmath>

Song::Song(const std::string& _path, const std::u8string& _filename, time_t _cr_time) {
    cr_time = _cr_time;
    id = rand();
    path = _path;
    filename = _filename;
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
    std::wstring s1 = f.tag()->title().toWString();
    std::wstring s2 = f.tag()->artist().toWString();
    std::wstring s3 = f.tag()->album().toWString();
    track = f.tag()->track();
    if (s1.size() == 0) {
        s1 = std::wstring(filename.begin(), filename.end());
    }
    title = s1;
    artist = s2;
    album = s3;
    ltitle = lower(s1);
    lalbum = lower(s2);
    lartist = lower(s3);
}

void Song::load_pic() {
    TagLib::MPEG::File ff(path.c_str());
    pic_loaded = true;
    time_loaded = clk.getElapsedTime();
    if (ff.hasID3v2Tag()) {
        TagLib::ID3v2::Tag* id3V2Tag = ff.ID3v2Tag();
        auto l = id3V2Tag->frameList("APIC");
        if (l.size() == 0) {
            return;
        }
        auto* pic = static_cast<TagLib::ID3v2::AttachedPictureFrame*>(l.front());
        int sz = std::sqrt(pic->picture().size() / 3);
        texture.loadFromMemory((const u_char*)pic->picture().data(), pic->picture().size(),
                               sf::IntRect(0, 0, 1200, 1200));
        //            texture.loadFromFile("/home/pavel/Music/amonger.png");
        texture.setSmooth(true);
        sprite.setTexture(texture);
        small_sprite.setTexture(texture);
        int x = texture.getSize().x;
        float scale = 300.f / x;
        sprite.setScale({scale, scale});
        scale *= 78.f / 300.f;
        small_sprite.setScale({scale, scale});
    }
}

std::wstring Song::lower(const std::wstring& s) {
    std::wstring result;
    std::locale loc = std::wcin.getloc();
    for (std::wstring::const_iterator it = s.begin(); it != s.end(); ++it) {
        result += std::use_facet<std::ctype<wchar_t>>(loc).tolower(*it);
    }
    return result;
}

void Song::get_fft(float* fft) { BASS_ChannelGetData(channel, fft, (int)BASS_DATA_FFT4096); }