#include "DirectoryNode.h"

void DirectoryNode::collectNames(std::vector<std::pair<std::string, int>>& result, int level) {
    result.emplace_back(name, level);
    for (DirectoryNode* child : children) {
        child->collectNames(result, level + 1);
    }
}

DirectoryNode::~DirectoryNode() {
    for (DirectoryNode* child : children) {
        delete child;
    }
}

void DirectoryNode::addChild(const std::string& path) {}
