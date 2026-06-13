#pragma once

#include "config/config.hpp"

#include <atomic>

class Application {
public:
    explicit Application(const Config& config);
    int run();
    void stop();

private:
    Config config_;
    std::atomic<bool> running_{true};
};