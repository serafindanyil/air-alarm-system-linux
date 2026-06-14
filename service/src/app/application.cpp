#include "application.hpp"

#include <chrono>
#include <iostream>
#include <thread>

Application::Application(const Config& config, const RegionList& region_list, int region_id)
    : config_(config), region_list_(region_list), region_id_(region_id) {}

int Application::run() {
    std::cout << "Air Alarm Service started" << std::endl;

    while (running_) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << "Config info: " << config_.api.url << std::endl;
        std::cout << "Region ID: " << region_id_ << std::endl;
    }

    std::cout << "Air Alarm Service stopped" << std::endl;

    return 0;
}

void Application::stop() {
    running_ = false;
}