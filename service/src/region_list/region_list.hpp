#pragma once

#include <string>
#include <unordered_map>
#include <vector>

struct RegionList {
    std::unordered_map<int, std::string> regions;
    std::unordered_map<int, std::vector<int>> adjacency_list;
};

class RegionListLoader {
public:
    static RegionList load(const std::string& pathname);

private:
    static void validate(const RegionList& list);
};