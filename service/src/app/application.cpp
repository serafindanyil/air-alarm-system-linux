#include "application.hpp"

#include <chrono>
#include <iostream>
#include <thread>

Application::Application(const Config& config, const RegionList& region_list)
    : config_(config), region_list_(region_list) {}

int Application::run() {
    std::cout << "Air Alarm Service started" << std::endl;

    while (running_) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << "Config info: " << config_.api.url << std::endl;
    }

    std::cout << "Air Alarm Service stopped" << std::endl;

    return 0;
}

void Application::stop() {
    running_ = false;
}