#ifndef XYLON_DOWNLOADVIEW_H
#define XYLON_DOWNLOADVIEW_H

#include <SFML/Graphics/Drawable.hpp>
#include "../utils/Utils.h"

class DownloadView : public sf::Drawable {
public:
    enum class DownloadState {
        SUCCESS, WAIT, FAIL, WRONG
    };

    static DownloadState state;
    static sf::Time last;
    static bool need_update;

    static void launch(const std::string& command);

    static void download(const std::string& link, const std::string& path);

    DownloadView();

    ~DownloadView() override;

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

#endif  // XYLON_DOWNLOADVIEW_H
