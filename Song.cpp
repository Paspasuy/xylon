#ifndef SONG
#define SONG

#include <SFML/System/Time.hpp>
#include "bass.h"

class Song {
    HCHANNEL channel;

public:
    std::string path;

    Song(const std::string &_path) {
        path = _path;
        channel = BASS_StreamCreateFile(FALSE, path.c_str(), 0, 0, 0);
        int err = BASS_ErrorGetCode();
        if (err != 0) {
            std::cerr << BASS_ErrorGetCode() << std::endl;
            std::cerr << "CHANNEL: " << channel << std::endl;
            std::cerr << "SONG: " << path << std::endl;
        }
//        music.setVolume(100.f);
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


};

#endif