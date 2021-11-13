#include "Player.cpp"

class SongSearch {
public:
    Player *p, *p2 = nullptr;
    std::wstring text;

    SongSearch(Player *_p) {
        p = _p;
    }

    Player* add_char(wchar_t c) {
        if (size(text) == 0) {
//            swap(p, p2);
        }
        text += c;
        std::vector<Song*>::iterator iter = p->songs.begin();
        std::vector<std::vector<Song*>::iterator> v;
        while (iter != p->songs.end()) {
            if ((*iter) -> matches(text)) {
                std::cerr << "OKAY: " << text << std::endl;
                v.push_back(iter);
            }
            ++iter;
        }
        if (v.empty()) {
            if (p2 == nullptr)
                return p;
            return p2;
        }
//        delete p2;
        p2 = new Player(p);
        for (auto it: v) {
            p2->songs.push_back(*it);
            if (p->ptr == it - p->songs.begin()) {
                p2->ptr = int(p2->songs.size()) - 1;
            }
        }
        return p2;
    }
    Player* pop_char() {
        if (text.size() != 0) {
            text.pop_back();
        }
        if (text.empty() == 0) {
            return p;
        }
        std::vector<Song*>::iterator iter = p->songs.begin();
        std::vector<std::vector<Song*>::iterator> v;
        while (iter != p->songs.end()) {
            if ((*iter) -> matches(text)) {
                std::cerr << "OKAY: " << text << std::endl;
                v.push_back(iter);
            }
            ++iter;
        }
        if (v.empty()) {
            if (p2 == nullptr)
                return p;
            return p2;
        }
//        delete p2;
        p2 = new Player(p);
        for (auto it: v) {
            p2->songs.push_back(*it);
            if (p->ptr == it - p->songs.begin()) {
                p2->ptr = int(p2->songs.size()) - 1;
            }
        }
        return p2;
    }
    Player* clear() {
//        if (text.empty())
//            return p;
        text.clear();
        while (text.size()) {
            text.pop_back();
        }
        p2 = nullptr;
        return p;
        //TODO: implement
    }
//TODO: implement del char
    void render(sf::RenderWindow &window, sf::Font &font) {
        sf::Text txt(text, font, 20);
        txt.setPosition(0, window.getSize().y-30);
        window.draw(txt);

//TODO: implement
    }
};