#include "application.hpp"
#include "logging/logging.hpp"

#include <chrono>
#include <thread>

Logging logger("APPLICATION");
Logging info_logger("INFO");
Logging led_logger("LED");

Application::Application(Config& config, LedController& led_controller, Api& api,
                         Logging& runtime_error_logger)
    : config_(config),
      led_controller_(led_controller),
      api_(api),
      runtime_error_logger_(runtime_error_logger) {}

int Application::run() {
    changeState(AppState::STARTING);

    while (running_) {
        const AppState new_state = evaluateState();

        applyState(new_state);

        std::this_thread::sleep_for(std::chrono::seconds(config_.api.poll_interval_seconds));
    }

    changeState(AppState::STOPPING);

    return 0;
}

AppState Application::evaluateState() {
    try {
        api_.refresh();

        if (api_.hasAlertCurrentRegion()) {
            return AppState::ALERT_CURRENT_REGION;
        }

        if (api_.hasAlertNeighboringRegions()) {
            return AppState::ALERT_NEIGHBORING_REGIONS;
        }

        return AppState::NO_ALERT;
    } catch (const std::exception& exception) {
        runtime_error_logger_.print(std::string(exception.what()));
        return AppState::STOPPING;
    }
}

void Application::applyState(AppState new_state) {
    if (state_ == new_state) {
        return;
    }

    state_ = new_state;
    changeState(state_);
}

void Application::changeState(AppState next_state) {
    switch (next_state) {
        case AppState::STARTING:
            logger.print("Launching Air Alarm Service...");
            break;
        case AppState::NO_INTERNET:
            led_controller_.active();
            info_logger.print("No internet connection");
            break;
        case AppState::BROKEN_API:
            led_controller_.active();
            led_logger.print("API is not working");
            break;
        case AppState::NO_ALERT:
            led_controller_.disabled();
            led_logger.print("No alerts");
            break;
        case AppState::ALERT_CURRENT_REGION:
            led_controller_.blink(config_.led.active_alarm_blink_hz);
            led_logger.print("Alert in current region");
            break;
        case AppState::ALERT_NEIGHBORING_REGIONS:
            led_controller_.blink(config_.led.neighbor_alarm_blink_hz);
            led_logger.print("Alert in neighboring regions");
            break;
        case AppState::STOPPING:
            logger.print("Stopping Air Alarm Service...");
            break;
        default:
            break;
    }
}

void Application::stop() {
    running_ = false;
}