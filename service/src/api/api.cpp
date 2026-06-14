#include "api.hpp"

#include <cpr/cpr.h>
#include <nlohmann/json.hpp>

#include <stdexcept>

using Json = nlohmann::json;

Api::Api(const std::string& api_url, int current_region_id, const RegionList& region_list)
    : api_url_(api_url), current_region_id_(current_region_id), region_list_(region_list) {}

void Api::refresh() {
    const auto response = cpr::Get(cpr::Url{api_url_}, cpr::Timeout{3000});

    if (response.error) {
        throw std::runtime_error("API request failed: " + response.error.message);
    }

    if (response.status_code != 200) {
        throw std::runtime_error("API returned status code: " +
                                 std::to_string(response.status_code));
    }

    parseResponse(response.text);
}

bool Api::hasAlertCurrentRegion() const {
    const auto it = alerts_.find(current_region_id_);

    if (it == alerts_.end()) {
        return false;
    }

    return it->second;
}

bool Api::hasAlertNeighboringRegions() const {
    const auto neighbors = RegionListLoader::getAdjacentRegionIds(region_list_, current_region_id_);

    for (int neighbor_id : neighbors) {
        const auto it = alerts_.find(neighbor_id);

        if (it != alerts_.end() && it->second) {
            return true;
        }
    }

    return false;
}

void Api::parseResponse(const std::string& response) {
    alerts_.clear();

    const Json json = Json::parse(response);

    const auto& states = json.at("states");

    for (const auto& [region_id, region_name] : region_list_.regions) {
        const auto it = states.find(region_name);

        alerts_[region_id] = it->at("alertnow").get<bool>();
    }
}