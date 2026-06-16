#include "application.hpp"

#include <chrono>
#include <iostream>
#include <thread>

Application::Application(Config& config, LedController& led_controller, Api& api)
    : config_(config), led_controller_(led_controller), api_(api) {}

int Application::run() {
    std::cout << "[Air Alarm Service] Launching Air Alarm Service..." << std::endl;

    while (running_) {
        const AppState new_state = evaluateState();

        applyState(new_state);

        std::this_thread::sleep_for(std::chrono::seconds(config_.api.poll_interval_seconds));
    }

    std::cout << "[Air Alarm Service] Stopping Air Alarm Service..." << std::endl;

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
        std::cerr << "[Air Alarm Service][RUNTIME_ERROR]: " << exception.what() << '\n';
        return AppState::RUNTIME_ERROR;
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
        case AppState::RUNTIME_ERROR:
        case AppState::NO_INTERNET:
        case AppState::BROKEN_API:
            led_controller_.active();
            break;
        case AppState::NO_ALERT:
            led_controller_.disabled();
            break;
        case AppState::ALERT_CURRENT_REGION:
            led_controller_.blink(config_.led.active_alarm_blink_hz);
            break;
        case AppState::ALERT_NEIGHBORING_REGIONS:
            led_controller_.blink(config_.led.neighbor_alarm_blink_hz);
            break;
        case AppState::STOPPING:
            break;
        default:
            break;
    }
}

void Application::stop() {
    running_ = false;
}