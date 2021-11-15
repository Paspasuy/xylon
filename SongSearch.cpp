#include "Player.cpp"

class SongSearch {
public:
    Player *p, *p2 = nullptr;
    std::wstring text;

    SongSearch(Player *_p) {
        p = _p;
    }

    Player* rebuild() {
        if (text.empty()) {
            return p;
        }
        std::vector<Song *>::iterator iter = p->songs.begin();
        std::vector<std::vector<Song *>::iterator> v;
        while (iter != p->songs.end()) {
            if ((*iter)->matches(text)) {
                v.push_back(iter);
            }
            ++iter;
        }
        if (v.empty()) {
            if (p2 == nullptr)
                return p;
            return p2;
        }
        if (p2 != nullptr)
            delete p2;
        p2 = new Player(p);
        for (auto it: v) {
            p2->songs.push_back(*it);
        }
        return p2;
    }

    Player *add_char(wchar_t c) {
        if (size(text) == 0) {}
        std::locale loc = std::wcin.getloc();
        text += std::use_facet<std::ctype<wchar_t>>(loc).tolower(c);
        return rebuild();
    }

    Player *pop_char() {
        if (text.size() != 0) {
            text.pop_back();
        }
        return rebuild();
    }

    Player *pop_word() {
        if (text.size() && text.back() == ' ') {
            text.pop_back();
        }
        while (text.size() != 0 && text.back() != ' ') {
            text.pop_back();
        }
        return rebuild();
    }

    Player *clear() {
        text.clear();
        delete p2;
        p2 = nullptr;
        return p;
    }

    void render(sf::RenderWindow &window, sf::Font &font) {
        if (!text.empty()) {
            sf::Text txt(L"search: " + text, font, 20);
            sf::RectangleShape back(sf::Vector2f((text.size() + 8) * 12 + 4, 20));
            txt.setPosition(0, window.getSize().y - 30);
            back.setPosition(txt.getPosition());
            back.setFillColor(sf::Color(32, 32, 32));
            window.draw(back);
            window.draw(txt);
        }
    }

    bool state() {
        return !text.empty();
    }
};