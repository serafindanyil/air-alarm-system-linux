#include "application.hpp"

#include <chrono>
#include <iostream>
#include <thread>

Application::Application(const Config& config, const Api& api) : config_(config), api_(api) {}

int Application::run() {
    std::cout << "Air Alarm Service started" << std::endl;

    while (running_) {
        api_.refresh();
        std::cout << "Alarm in current region: " << api_.hasAlertCurrentRegion() << std::endl;
        std::cout << "Alarm in neighboring regions: " << api_.hasAlertNeighboringRegions()
                  << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(config_.api.poll_interval_seconds));
    }

    std::cout << "Air Alarm Service stopped" << std::endl;

    return 0;
}

void Application::stop() {
    running_ = false;
}