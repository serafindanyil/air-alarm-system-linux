#include "api.hpp"

#include <cpr/cpr.h>
#include <logging/logging.hpp>
#include <nlohmann/json.hpp>

using Json = nlohmann::json;

Logging api_error_logger("API");

Api::Api(const std::string& api_url, int current_region_id, const RegionList& region_list)
    : api_url_(api_url), current_region_id_(current_region_id), region_list_(region_list) {}

void Api::refresh() {
    const auto response = cpr::Get(cpr::Url{api_url_}, cpr::Timeout{3000});

    status_code_ = response.status_code;

    if (response.error) {
        api_error_logger.print("Response error: " + response.error.message);
    }

    if (response.status_code != 200) {
        api_error_logger.print("Status code: " + std::to_string(response.status_code));
    }

    if (response.status_code == 400) {
        api_error_logger.print("Bad request");
    }

    if (response.status_code == 404) {
        api_error_logger.print("Resource not found");
    }

    if (response.status_code == 500) {
        api_error_logger.print("Server error");
    }

    if (response.text.empty()) {
        api_error_logger.print("Response body is empty");
        status_code_ = 0;
        return;
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

int Api::getStatusCode() const {
    return status_code_;
}