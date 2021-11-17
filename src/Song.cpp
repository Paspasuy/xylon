#include "Song.h"

Song::Song(const std::string &_path, const std::u8string &_filename) {
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

    void Song::play() {
        BASS_ChannelPlay(channel, FALSE);
    }

void Song::pause() {
    BASS_ChannelPause(channel);
}

void Song::stop() {
    BASS_ChannelPause(channel);
    BASS_ChannelSetPosition(channel, 0, BASS_POS_BYTE);
}

void Song::set_vol(double x) {
    BASS_ChannelSetAttribute(channel, BASS_ATTRIB_VOL, x);
}

sf::Time Song::getDuration() {
    QWORD len = BASS_ChannelGetLength(channel, BASS_POS_BYTE); // the length in bytes
    double time = BASS_ChannelBytes2Seconds(channel, len); // the length in seconds
    return sf::microseconds(int64_t(time * 1000 * 1000));
}

sf::Time Song::getPlayingOffset() {
    QWORD len = BASS_ChannelGetPosition(channel, BASS_POS_BYTE); // the length in bytes
    double time = BASS_ChannelBytes2Seconds(channel, len); // the length in seconds
    return sf::microseconds(int64_t(time * 1000 * 1000));
}

void Song::backward_5() {
    QWORD len = BASS_ChannelGetPosition(channel, BASS_POS_BYTE); // the length in bytes
    double time = BASS_ChannelBytes2Seconds(channel, len); // the length in seconds
    time = std::max(0., time - 5);
    BASS_ChannelSetPosition(channel, BASS_ChannelSeconds2Bytes(channel, time), BASS_POS_BYTE);
}

void Song::forward_5() {
    QWORD len = BASS_ChannelGetPosition(channel, BASS_POS_BYTE); // the length in bytes
    double time = BASS_ChannelBytes2Seconds(channel, len); // the length in seconds
    time = std::min(double(getDuration().asSeconds()), time + 5);
    BASS_ChannelSetPosition(channel, BASS_ChannelSeconds2Bytes(channel, time), BASS_POS_BYTE);
}

void Song::set_position(double pos) {
    BASS_ChannelSetPosition(channel, BASS_ChannelSeconds2Bytes(channel, pos), BASS_POS_BYTE);
}

bool Song::matches(std::wstring text) {
    if (ltitle.find(text) != std::string::npos) return 1;
    if (lartist.find(text) != std::string::npos) return 1;
    if (lalbum.find(text) != std::string::npos) return 1;
    return 0;
}

void Song::add_meta() {
    TagLib::FileRef f(path.c_str());
    std::wstring s1 = f.tag()->title().toWString();
    std::wstring s2 = f.tag()->artist().toWString();
    std::wstring s3 = f.tag()->album().toWString();
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
    std::cerr << path << std::endl;
    TagLib::MPEG::File ff(path.c_str());
    pic_loaded = 1;
    if (ff.hasID3v2Tag()) {
        TagLib::ID3v2::Tag *id3V2Tag = ff.ID3v2Tag();
        auto l = id3V2Tag->frameList("APIC");
        if (l.size() == 0) {
            return;
        }
        auto *pic = static_cast<TagLib::ID3v2::AttachedPictureFrame *>(l.front());
        texture.loadFromMemory((const u_char *) pic->picture().data(), pic->picture().size(),
                               sf::IntRect(0, 0, 1200, 1200));
//            texture.loadFromFile("/home/pavel/Music/amonger.png");
        texture.setSmooth(true);
        sprite.setTexture(texture);
        sprite.setScale({0.25, 0.25});
    }
}

std::wstring Song::lower(const std::wstring &s) {
    std::wstring result;
    std::locale loc = std::wcin.getloc();
    for (std::wstring::const_iterator it = s.begin(); it != s.end(); ++it) {
        result += std::use_facet<std::ctype<wchar_t>>(loc).tolower(*it);
    }
    return result;
}
