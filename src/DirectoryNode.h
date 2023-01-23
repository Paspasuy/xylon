#ifndef XYLON_DIRECTORYNODE_H
#define XYLON_DIRECTORYNODE_H

#include <string>
#include <vector>

struct DirectoryNode {
    std::vector<DirectoryNode*> children;
    std::string name;
    void addChild(const std::string& path);
    void collectNames(std::vector<std::pair<std::string, int>>& result, int level = 0);
    ~DirectoryNode();
};

#endif  // XYLON_DIRECTORYNODE_H
