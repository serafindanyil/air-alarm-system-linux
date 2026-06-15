#pragma once

#include "api/api.hpp"
#include "config/config.hpp"
#include "gpio/gpio.hpp"

#include <atomic>

class Application {
public:
    explicit Application(const Config& config, Gpio& gpio, Api& api);
    int run();
    void stop();

private:
    Config config_;
    Gpio& gpio_;
    Api& api_;
    std::atomic<bool> running_{true};
};