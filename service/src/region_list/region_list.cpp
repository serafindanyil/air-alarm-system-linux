#include "region_list.hpp"

#include <algorithm>
#include <fstream>
#include <stdexcept>
#include <unordered_set>

#include <nlohmann/json.hpp>

using Json = nlohmann::json;

RegionList RegionListLoader::load(const std::string& pathname) {
    std::ifstream file(pathname);

    if (!file.is_open()) {
        throw std::runtime_error("Failed to open region list: " + pathname);
    }

    Json json;
    file >> json;

    if (!json.contains("regions")) {
        throw std::runtime_error("Region list missing field: regions");
    }

    if (!json.contains("regions_adjacency_list")) {
        throw std::runtime_error("Region list missing field: regions_adjacency_list");
    }

    RegionList list;

    for (const auto& [id_string, name_json] : json.at("regions").items()) {
        int id = std::stoi(id_string);
        list.regions[id] = name_json.get<std::string>();
    }

    for (const auto& [id_string, neighbors_json] : json.at("regions_adjacency_list").items()) {
        int id = std::stoi(id_string);
        for (const auto& neighbor_json : neighbors_json) {
            list.adjacency_list[id].push_back(neighbor_json.get<int>());
        }
    }

    validate(list);

    return list;
}

void RegionListLoader::validate(const RegionList& list) {
    if (list.regions.empty() || list.adjacency_list.empty()) {
        throw std::runtime_error("Region list or adjacency list is empty");
    }

    std::unordered_set<std::string> region_names;

    for (const auto& [id, name] : list.regions) {
        if (id <= 0) {
            throw std::runtime_error("Region id must be positive");
        }
        if (name.empty()) {
            throw std::runtime_error("Region name is empty");
        }
        if (!region_names.insert(name).second) {
            throw std::runtime_error("Duplicate region name: " + name);
        }
        if (!list.adjacency_list.contains(id)) {
            throw std::runtime_error("Missing adjacency list for region id: " + std::to_string(id));
        }
    }

    for (const auto& [region_id, neighbors] : list.adjacency_list) {
        if (!list.regions.contains(region_id)) {
            throw std::runtime_error("Adjacency list contains unknown region id: " +
                                     std::to_string(region_id));
        }

        std::unordered_set<int> unique_neighbors;

        for (int neighbor_id : neighbors) {
            if (neighbor_id == region_id) {
                throw std::runtime_error("Region cannot be neighbor of itself: " +
                                         std::to_string(region_id));
            }
            if (!list.regions.contains(neighbor_id)) {
                throw std::runtime_error("Unknown neighbor id: " + std::to_string(neighbor_id));
            }
            if (!unique_neighbors.insert(neighbor_id).second) {
                throw std::runtime_error("Duplicate neighbor id " + std::to_string(neighbor_id) +
                                         " for region " + std::to_string(region_id));
            }

            const auto& reverse_neighbors = list.adjacency_list.at(neighbor_id);
            if (std::find(reverse_neighbors.begin(), reverse_neighbors.end(), region_id) ==
                reverse_neighbors.end()) {
                throw std::runtime_error("Missing reverse connection between " +
                                         std::to_string(region_id) + " and " +
                                         std::to_string(neighbor_id));
            }
        }
    }
}