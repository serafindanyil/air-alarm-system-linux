#pragma once

#include "api/api.hpp"
#include "config/config.hpp"
#include "led_controller/led_controller.hpp"

#include <atomic>

class Application {
public:
    explicit Application(const Config& config, LedController& led_controller, Api& api);

    int run();
    void stop();

private:
    Config config_;
    LedController& led_controller_;
    Api& api_;
    std::atomic<bool> running_{true};
};