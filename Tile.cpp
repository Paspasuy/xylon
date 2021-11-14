#ifndef TILE
#define TILE

#include "Song.cpp"
#include <taglib/fileref.h>
#include <taglib/mpegfile.h>
#include <taglib/attachedpictureframe.h>
#include <taglib/id3v2tag.h>
#include <taglib/tag.h>

typedef unsigned char uchar;
const int MIN_W = 400;

class Tile {
public:
    static const int PIC = 300;
    static int W;
    static const int H = 80;
    sf::Vector2i position;
    sf::Sprite sprite;
    sf::Texture texture;
    bool pic_loaded = 0;
    bool has_pic = 0;
    Song *s;
    int margin = 0;

    Tile(Song *_s) {
        s = _s;
//        TagLib::MP4::Tag* tag = ff.tag();
//        auto itemsListMap = tag->itemListMap();
//        TagLib::MP4::Item coverItem = itemsListMap["covr"];
//        TagLib::MP4::CoverArtList coverArtList = coverItem.toCoverArtList();
    }

    static std::wstring lower(const std::wstring &s) {
        std::wstring result;
        std::locale loc = std::wcin.getloc();
        for (std::wstring::const_iterator it = s.begin(); it != s.end(); ++it) {
            result += std::use_facet<std::ctype<wchar_t>>(loc).tolower(*it);
        }
        return result;
    }

    static void add_meta(Song *s) {
        TagLib::FileRef f((s->path).c_str());
        std::wstring s1 = f.tag()->title().toWString();
        std::wstring s2 = f.tag()->artist().toWString();
        std::wstring s3 = f.tag()->album().toWString();
        if (s1.size() == 0) {
            s1 = std::wstring(s->path.begin(), s->path.end());
        }
        s->title = s1;
        s->artist = s2;
        s->album = s3;
        s->ltitle = lower(s1);
        s->lalbum = lower(s2);
        s->lartist = lower(s3);
    }

    void load_pic() {
        TagLib::MPEG::File ff((s->path).c_str());
        pic_loaded = 1;
        if (ff.hasID3v2Tag()) {
            has_pic = 1;
            TagLib::ID3v2::Tag *id3V2Tag = ff.ID3v2Tag();
            auto l = id3V2Tag->frameList("APIC");
            auto *pic = static_cast<TagLib::ID3v2::AttachedPictureFrame *>(l.front());
            texture.loadFromMemory((const uchar *) pic->picture().data(), pic->picture().size(),
                                   sf::IntRect(0, 0, 1200, 1200));
//            texture.loadFromFile("/home/pavel/Music/amonger.png");
            texture.setSmooth(true);
            sprite.setTexture(texture);
            sprite.setScale({0.25, 0.25});
        }
    }

    void render_pic(sf::RenderWindow &window) {
        margin = std::min(std::max(0, int(window.getSize().x - Tile::W - PIC)) / 2,
                          std::max(0, int(window.getSize().y - PIC)));
        sprite.setPosition(margin, margin / 2);
//            sprite.setPosition(position.x + Tile::W - 100, position.y);
        window.draw(sprite);

    }

    void render(sf::RenderWindow &window, sf::Font &font, sf::Font &bold_font, sf::RectangleShape &sh, bool is_cur) {
        if (is_cur && !pic_loaded) {
            load_pic();
        }
        sh.setPosition(position.x, position.y);
        sh.setOutlineThickness(2.f);
        sh.setFillColor(sf::Color(sf::Color::Transparent));
        sf::Text title_text(s->title, bold_font, 16);
        sf::Text artist_text(s->artist, font, 14);
        sf::Text album_text(s->album, font, 14);
        title_text.setPosition(sh.getPosition().x + 2, sh.getPosition().y + 2);
        artist_text.setPosition(sh.getPosition().x + 2, sh.getPosition().y + 20);
        album_text.setPosition(sh.getPosition().x - 4 - s->album.getSize() * 8 + Tile::W, sh.getPosition().y + Tile::H - 20);
        window.draw(sh);
        window.draw(title_text);
        window.draw(artist_text);
        window.draw(album_text);
        if (is_cur) {
            render_pic(window);
            int w = 10;
            int sw = margin + (PIC - w * s->title.getSize()) / 2;
            title_text.setPosition(sw, PIC + margin * 0.5 + 12);
            window.draw(title_text);
            w = 8;
            sw = margin + (PIC - w * s->artist.getSize()) / 2;
            artist_text.setPosition(sw, PIC + margin * 0.5 + 20 + 12);
            window.draw(artist_text);
        }
    }
};

int Tile::W;
#endif
