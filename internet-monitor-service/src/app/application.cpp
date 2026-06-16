#include "application.hpp"

#include <chrono>
#include <cstdlib>
#include <thread>

#include "logging/logging.hpp"

Application::Application() = default;

Logging logger("APPLICATION");

int Application::run() {
    logger.print("Starting Internet Monitor...");

    while (running_) {
        has_internet_connection_ = checkInternetConnection();

        if (has_internet_connection_) {
            logger.print("Internet connection is available");
        } else {
            logger.print("Internet connection is unavailable");
        }

        std::this_thread::sleep_for(std::chrono::seconds(ping_poll_interval_seconds_));
    }

    logger.print("Stopping Internet Monitor...");

    return 0;
}

void Application::stop() {
    running_ = false;
}

bool Application::hasInternetConnection() const {
    return has_internet_connection_;
}

bool Application::checkInternetConnection() const {
    const int result = std::system("ping -c 1 -W 1 8.8.8.8 > /dev/null 2>&1");

    return result == 0;
}