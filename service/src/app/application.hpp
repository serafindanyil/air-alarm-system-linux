#pragma once

#include "api/api.hpp"
#include "config/config.hpp"
#include "led_controller/led_controller.hpp"
#include "logging/logging.hpp"

#include <atomic>

enum class AppState {
    STARTING,
    NO_INTERNET,
    BROKEN_API,
    NO_ALERT,
    ALERT_CURRENT_REGION,
    ALERT_NEIGHBORING_REGIONS,
    STOPPING,
};

class Application {
public:
    explicit Application(Config& config, LedController& led_controller, Api& api,
                         Logging& runtime_error_logger);

    int run();
    void stop();

private:
    Config& config_;
    LedController& led_controller_;
    Api& api_;

    Logging& runtime_error_logger_;

    AppState state_{AppState::STARTING};
    AppState evaluateState();

    void applyState(AppState state);
    void changeState(AppState next_state);

    std::atomic<bool> running_{true};
};