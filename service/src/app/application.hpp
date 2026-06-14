#pragma once

#include "config/config.hpp"
#include "region_list/region_list.hpp"

#include <atomic>

class Application {
public:
    explicit Application(const Config& config, const RegionList& region_list);
    int run();
    void stop();

private:
    Config config_;
    RegionList region_list_;
    std::atomic<bool> running_{true};
};