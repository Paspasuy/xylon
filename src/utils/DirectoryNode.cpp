#include "DirectoryNode.h"

#include <filesystem>

void DirectoryNode::collectNames(std::vector<std::wstring>& result, int level) {
    if (level >= 0) {
        std::string tab;
        for (int i = 0; i < level; ++i) {
            tab += "    ";
        }
        result.emplace_back(
            std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(tab + name));
    }
    for (DirectoryNode* child : children) {
        child->collectNames(result, level + 1);
    }
}

DirectoryNode::~DirectoryNode() {
    for (DirectoryNode* child : children) {
        delete child;
    }
}

void DirectoryNode::addChild(const std::string& path) {
    children.emplace_back(new DirectoryNode(path));
    using iter = std::filesystem::directory_iterator;
    for (const auto& dirEntry : iter(path)) {
        if (dirEntry.is_directory()) {
            children.back()->addChild(dirEntry.path());
        }
    }
}

DirectoryNode::DirectoryNode(const std::string& name) : name(name) {}
