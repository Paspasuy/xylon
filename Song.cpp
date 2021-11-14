#ifndef SONG
#define SONG

#include <SFML/System/Time.hpp>
#include <SFML/System/String.hpp>
#include "bass.h"

class Song {
    HCHANNEL channel;

public:
    std::string path;
    sf::String title = "Title";
    sf::String artist = "John Doe";
    sf::String album = "Default Album";
    std::wstring ltitle = L"title";
    std::wstring lartist = L"john doe";
    std::wstring lalbum = L"default album";
    uint64_t id;

    Song(const std::string &_path) {
        id = rand();
        path = _path;
        channel = BASS_StreamCreateFile(FALSE, path.c_str(), 0, 0, 0);
        int err = BASS_ErrorGetCode();
        if (err != 0) {
            std::cerr << err << std::endl;
            std::cerr << "CHANNEL: " << channel << std::endl;
            std::cerr << "SONG: " << path << std::endl;
        }
    }

    void play() {
        BASS_ChannelPlay(channel, FALSE);
    }

    void pause() {
        BASS_ChannelPause(channel);
    }

    void stop() {
        BASS_ChannelPause(channel);
        BASS_ChannelSetPosition(channel, 0, BASS_POS_BYTE);
    }

    void set_vol(double x) {
        BASS_ChannelSetAttribute(channel, BASS_ATTRIB_VOL, x);
    }

    sf::Time getDuration() {
        QWORD len = BASS_ChannelGetLength(channel, BASS_POS_BYTE); // the length in bytes
        double time = BASS_ChannelBytes2Seconds(channel, len); // the length in seconds
        return sf::microseconds(int64_t(time * 1000 * 1000));
//        return sf::Time();
    }

    sf::Time getPlayingOffset() {
        QWORD len = BASS_ChannelGetPosition(channel, BASS_POS_BYTE); // the length in bytes
        double time = BASS_ChannelBytes2Seconds(channel, len); // the length in seconds
        return sf::microseconds(int64_t(time * 1000 * 1000));
    }

    void backward_5() {
        QWORD len = BASS_ChannelGetPosition(channel, BASS_POS_BYTE); // the length in bytes
        double time = BASS_ChannelBytes2Seconds(channel, len); // the length in seconds
        time = std::max(0., time - 5);
        BASS_ChannelSetPosition(channel, BASS_ChannelSeconds2Bytes(channel, time), BASS_POS_BYTE);
    }

    void forward_5() {
        QWORD len = BASS_ChannelGetPosition(channel, BASS_POS_BYTE); // the length in bytes
        double time = BASS_ChannelBytes2Seconds(channel, len); // the length in seconds
        time = std::min(double(getDuration().asSeconds()), time + 5);
        BASS_ChannelSetPosition(channel, BASS_ChannelSeconds2Bytes(channel, time), BASS_POS_BYTE);
    }

    bool matches(std::wstring text) {
        if (ltitle.find(text) != std::string::npos) return 1;
        if (lartist.find(text) != std::string::npos) return 1;
        if (lalbum.find(text) != std::string::npos) return 1;
        return 0;
    }

};

#endif