#include "region_list.hpp"

#include <fstream>
#include <stdexcept>

#include <nlohmann/json.hpp>

using Json = nlohmann::json;

RegionList RegionListLoader::load(const std::string& pathname) {
    std::ifstream file(pathname);

    if (!file.is_open()) {
        throw std::runtime_error("Failed to open region list: " + pathname);
    }

    Json json;
    file >> json;

    RegionList list;

    for (const auto& [id_string, name_json] : json.at("regions").items()) {
        const int id = std::stoi(id_string);
        list.regions[id] = name_json.get<std::string>();
    }

    for (const auto& [id_string, neighbors_json] : json.at("regions_adjacency_list").items()) {
        const int id = std::stoi(id_string);

        for (const auto& neighbor_json : neighbors_json) {
            list.adjacency_list[id].push_back(neighbor_json.get<int>());
        }
    }

    return list;
}

bool RegionListLoader::containsRegion(const RegionList& list, const std::string& region_name) {
    for (const auto& [id, name] : list.regions) {
        if (name == region_name) {
            return true;
        }
    }

    return false;
}

int RegionListLoader::getRegionId(const RegionList& list, const std::string& region_name) {
    for (const auto& [id, name] : list.regions) {
        if (name == region_name) {
            return id;
        }
    }

    throw std::runtime_error("Unknown region: " + region_name);
}

std::vector<int> RegionListLoader::getAdjacentRegionIds(const RegionList& list, int region_id) {
    return list.adjacency_list.at(region_id);
}