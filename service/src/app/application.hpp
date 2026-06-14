#pragma once

#include "api/api.hpp"
#include "config/config.hpp"

#include <atomic>

class Application {
public:
    explicit Application(const Config& config, const Api& api);
    int run();
    void stop();

private:
    Config config_;
    Api api_;
    std::atomic<bool> running_{true};
};