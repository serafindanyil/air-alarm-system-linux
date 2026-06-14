#pragma once

#include "region_list/region_list.hpp"

#include <string>
#include <unordered_map>

class Api {
public:
    Api(const std::string& api_url, int current_region_id, const RegionList& region_list);

    void refresh();

    bool hasAlertCurrentRegion() const;
    bool hasAlertNeighboringRegions() const;

private:
    std::string api_url_;
    int current_region_id_;
    RegionList region_list_;

    std::unordered_map<int, bool> alerts_;

    void parseResponse(const std::string& response);
};