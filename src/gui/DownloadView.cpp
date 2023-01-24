#include "DownloadView.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Text.hpp>
#include <thread>

const std::string S1 =
    "\
#!/bin/bash\n\
cd \"";
const std::string S2 =
    "\"\n\
yt-dlp -o '%(title)s' \"";
const std::string S3 =
    "\"\n\
name=$(ls -Art | grep .webm | tail -n 1)\n\
echo $name\n\
name_crop=${name%.*}\n\
echo $name_crop\n\
name_new=$name_crop'.mp3'\n\
echo $name_crop\n\
ffmpeg -i \"$name\" \"$name_new\"\n\
rm \"$name\"\n";

bool valid_link(const std::string& link) {
    return (link.find("youtube.com") != std::string::npos) ||
           (link.find("yt.be") != std::string::npos) ||
           (link.find("youtu.be") != std::string::npos);
}

DownloadView::~DownloadView() {}

void DownloadView::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    if (state != DownloadState::WAIT && clk.getElapsedTime() - last > sf::seconds(5)) {
        return;
    }
    sf::Text state_text;
    if (state == DownloadState::SUCCESS) {
        state_text = sf::Text("Done", BOLD_FONT, 16);
        state_text.setFillColor(sf::Color::Green);
    } else if (state == DownloadState::WAIT) {
        state_text = sf::Text("Downloading...", BOLD_FONT, 16);
        state_text.setFillColor(sf::Color::Yellow);
    } else if (state == DownloadState::FAIL) {
        state_text = sf::Text("Failed", BOLD_FONT, 16);
        state_text.setFillColor(sf::Color::Red);
    } else if (state == DownloadState::WRONG) {
        state_text = sf::Text("Bad link", BOLD_FONT, 16);
        state_text.setFillColor(sf::Color::Magenta);
    }
    state_text.setPosition(0, 0);
    target.draw(state_text);
}

void DownloadView::download(const std::string& link, const std::string& path) {
    last = clk.getElapsedTime();
    if (!valid_link(link)) {
        state = DownloadState::WRONG;
    } else {
        state = DownloadState::WAIT;
        std::thread thr(launch, S1 + path + S2 + link + S3);
        thr.detach();
    }
}

DownloadView::DownloadView() {}

void DownloadView::launch(const std::string& command) {
    int result = system(command.c_str());
    if (result == 0) {
        need_update = true;
        state = DownloadState::SUCCESS;
        last = clk.getElapsedTime();
    } else {
        state = DownloadState::FAIL;
    }
}

DownloadView::DownloadState DownloadView::state = DownloadView::DownloadState::SUCCESS;
bool DownloadView::need_update = false;
sf::Time DownloadView::last;
