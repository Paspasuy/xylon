#include <filesystem>
#include "DirectoryNode.h"

void DirectoryNode::collectNames(std::vector<std::string>& result, int level) {
    if (level >= 0) {
        std::string tab;
        for (int i = 0; i < level; ++i) {
            tab += "    ";
        }
        result.emplace_back(tab + name);
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
