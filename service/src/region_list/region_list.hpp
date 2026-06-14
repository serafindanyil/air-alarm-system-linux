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

    static bool containsRegion(const RegionList& list, const std::string& regionName);

    static int getRegionId(const RegionList& list, const std::string& regionName);

    static std::vector<int> getAdjacentRegionIds(const RegionList& list, int region_id);

private:
    static void validate(const RegionList& list);
};